#include "pch.h"

#include "App.h"

#include "AutolinkedNativeModules.g.h"
#include "ReactPackageProvider.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::ApplicationModel;
namespace winrt::ReactNativeNotes::implementation
{
/// <summary>
/// Initializes the singleton application object.  This is the first line of
/// authored code executed, and as such is the logical equivalent of main() or
/// WinMain().
/// </summary>
App::App() noexcept
{
#if BUNDLE
    JavaScriptBundleFile(L"index.windows");
    InstanceSettings().UseWebDebugger(false);
    InstanceSettings().UseFastRefresh(false);
#else
    JavaScriptBundleFile(L"index");
    InstanceSettings().UseWebDebugger(true);
    InstanceSettings().UseFastRefresh(true);
#endif

#if _DEBUG
    InstanceSettings().UseDeveloperSupport(true);
#else
    InstanceSettings().UseDeveloperSupport(false);
#endif

    RegisterAutolinkedNativeModulePackages(PackageProviders()); // Includes any autolinked modules

    PackageProviders().Append(make<ReactPackageProvider>()); // Includes all modules in this project
    PackageProviders().Append( winrt::DateTimePicker::ReactPackageProvider() );
    PackageProviders().Append( winrt::CheckboxWindows::ReactPackageProvider() );

    InitializeComponent();
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(activation::LaunchActivatedEventArgs const& e)
{
    super::OnLaunched( e );

    auto coreTitleBar = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView().TitleBar();
    coreTitleBar.ExtendViewIntoTitleBar( true );
    auto titleBar = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().TitleBar();
    titleBar.ButtonBackgroundColor( Windows::UI::Colors::Transparent() );
    titleBar.ForegroundColor( Windows::UI::Colors::Transparent() );
    titleBar.ButtonForegroundColor( Windows::UI::Colors::White() );
    auto buttonBackgroundColor = Windows::UI::Color();
    //FF5A2D
    buttonBackgroundColor.R = 0xFF;
    buttonBackgroundColor.G = 0x5A;
    buttonBackgroundColor.B = 0x2D;
    titleBar.ButtonHoverBackgroundColor( buttonBackgroundColor );
    Frame rootFrame = Window::Current().Content().as<Frame>();
    rootFrame.Navigate( xaml_typename<ReactNativeNotes::MainPage>(), box_value( e.Arguments() ) );
}

/// <summary>
/// Invoked when the application is activated by some means other than normal launching.
/// </summary>
void App::OnActivated(Activation::IActivatedEventArgs const &e) {
  auto preActivationContent = Window::Current().Content();
  super::OnActivated(e);
  if (!preActivationContent && Window::Current()) {
    Frame rootFrame = Window::Current().Content().as<Frame>();
    rootFrame.Navigate(xaml_typename<MainPage>(), nullptr);
  }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    // Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(IInspectable const&, NavigationFailedEventArgs const& e)
{
    throw hresult_error(E_FAIL, hstring(L"Failed to load Page ") + e.SourcePageType().Name);
}

} // namespace winrt::ReactNativeNotes::implementation
