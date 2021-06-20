// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

// include the render object interface to access object properties
#include <Engine/Rendering/RenderObjectManager.hpp>

// include the KeyMappingManager
#include <Gui/Utils/KeyMappingManager.hpp>
#include <Gui/Utils/Keyboard.hpp>

// include Qt components
#include <QKeyEvent>

#include <Gui/Viewer/Viewer.hpp>

/**
 * Simple custom windows for custom KeyEvent demonstration
 */
class DemoWindow : public Ra::Gui::SimpleWindow
{
    Q_OBJECT

  public:
    /// Reuse the SimpleWindow constructors
    using Ra::Gui::SimpleWindow::SimpleWindow;

    explicit DemoWindow( uint w = 800, uint h = 640, QWidget* parent = nullptr ) :
        Ra::Gui::SimpleWindow::SimpleWindow( w, h, parent ) {
        //! [Initialize KeyEvent context and actions for demo window]
        //! [Initialize KeyEvent context and actions for demo window]
    }
    void configure() override {

        DEMO_COLORUP = getViewer()->addKeyPressEventAction(
            "DEMO_COLORUP", "Key_F1", "", "", "false", [this]( QKeyEvent* event ) {
                this->colorup( event );
            } );
        //        DEMO_COLORDOWN =
        //            getViewer()->addKeyPressEventAction( "DEMO_COLORDOWN", "Key_F2", "", "",
        //            "false", colordown );
        if ( DEMO_COLORUP.isInvalid() || DEMO_COLORDOWN.isInvalid() )
        {
            LOG( Ra::Core::Utils::logERROR ) << "Error : invalid context or actions for custom"
                                             << "Color Up [" << DEMO_COLORUP
                                             << "], "
                                                "Color Down ["
                                             << DEMO_COLORDOWN << "]";
        }
    }
    /// Set the object to colorize
    void colorize( std::shared_ptr<Ra::Engine::Rendering::RenderObject> o ) { m_obj = o; }

    //! [Manage KeyEvent reaching the window]
    /// Custom Key event management method
    void colorup( QKeyEvent* event ) {
        auto& mesh = dynamic_cast<Ra::Core::Geometry::TriangleMesh&>(
            m_obj->getMesh()->getAbstractGeometry() );
        m_colIdx = ( m_colIdx + 1 ) % 10;
        mesh.colorize( m_colors[m_colIdx] );
    }
    void colordown( QKeyEvent* event ) {
        auto& mesh = dynamic_cast<Ra::Core::Geometry::TriangleMesh&>(
            m_obj->getMesh()->getAbstractGeometry() );
        m_colIdx = ( m_colIdx + 9 ) % 10;
        mesh.colorize( m_colors[m_colIdx] );
    }
    //! [Manage KeyEvent reaching the window]
  private:
    //! [KeyEvent for demo window]
    Ra::Gui::KeyMappingManager::Context m_demoContext {};

#define KeyMappingDemo        \
    KMA_VALUE( DEMO_COLORUP ) \
    KMA_VALUE( DEMO_COLORDOWN )

#define KMA_VALUE( x ) static Ra::Gui::KeyMappingManager::KeyMappingAction x;
    KeyMappingDemo
#undef KMA_VALUE

        //! [KeyEvent for demo window]

        std::shared_ptr<Ra::Engine::Rendering::RenderObject>
            m_obj;
    std::array<Ra::Core::Utils::Color, 10> m_colors {Ra::Core::Utils::Color::Green(),
                                                     Ra::Core::Utils::Color::Blue(),
                                                     Ra::Core::Utils::Color::Yellow(),
                                                     Ra::Core::Utils::Color::Magenta(),
                                                     Ra::Core::Utils::Color::Cyan(),
                                                     Ra::Core::Utils::Color::White(),
                                                     Ra::Core::Utils::Color::Grey(),
                                                     Ra::Core::Utils::Color::Black(),
                                                     Ra::Core::Utils::Color::Skin(),
                                                     Ra::Core::Utils::Color::Red()};
    int m_colIdx {0};
};

#define KMA_VALUE( x ) Ra::Gui::KeyMappingManager::KeyMappingAction DemoWindow::x;
KeyMappingDemo
#undef KMA_VALUE

    /**
     * Define a factory that set instanciate the Demonstration Window
     */
    class DemoWindowFactory : public Ra::Gui::BaseApplication::WindowFactory
{
  public:
    ~DemoWindowFactory() = default;
    inline Ra::Gui::MainWindowInterface* createMainWindow() const override {
        auto window = new DemoWindow();
        return window;
    }
};
#include "main.moc"

int main( int argc, char* argv[] ) {
    //! [Creating the application]
    Ra::Gui::BaseApplication app( argc, argv );
    app.initialize( DemoWindowFactory {} );
    //! [Creating the application]

    //! [Creating the cube]
    auto cube = Ra::Core::Geometry::makeSharpBox( {0.1f, 0.1f, 0.1f} );
    //! [Creating the cube]

    //! [Colorize the Cube]
    cube.addAttrib(
        "in_color",
        Ra::Core::Vector4Array {cube.vertices().size(), Ra::Core::Utils::Color::Green()} );
    //! [Colorize the Cube]

    //! [Create the engine entity for the cube]
    auto e = app.m_engine->getEntityManager()->createEntity( "Green cube" );
    //! [Create the engine entity for the cube]

    //! [Create a geometry component with the cube]
    auto c =
        new Ra::Engine::Scene::TriangleMeshComponent( "Cube Mesh", e, std::move( cube ), nullptr );
    //! [Create a geometry component with the cube]

    //! [Register the entity/component association to the geometry system ]
    auto geometrySystem = app.m_engine->getSystem( "GeometrySystem" );
    geometrySystem->addComponent( e, c );
    //! [Register the entity/component association to the geometry system ]

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

    //! [Tell the window to control the color of the cube with <F1> or <F2> key]
    auto appWindow = dynamic_cast<DemoWindow*>( app.m_mainWindow.get() );
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
        c->m_renderObjects[0] );
    appWindow->colorize( ro );
    //! [Tell the window to control the color of the cube with <F1> or <F2> key]

    return app.exec();
}
