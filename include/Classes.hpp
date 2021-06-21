#pragma once

namespace yaodaq
{

  enum class CLASS : std::uint_least8_t
  {
    //Generic Ones
    Web = 100,
    Unknown = 1,
    WebSocketServer = 2,
    Module = 3,
    Board = 4,

    //Specific
    Logger,
    Controller,
    Configurator,
    SlowController,
    Viewer,
    Analyser,
    FileWritter,

    //Use this if it's a mix of

    WebBrowser = Web+1,
    //Now the browsers
    WebLogger = Web+Logger,
    WebController = Web+Controller,
    WebConfigurator = Web+Configurator,
    WebSlowController = Web+SlowController,
    WebViewer = Web+Viewer,
    WebAnalyser = Web+Analyser,
    WebFileWritter = Web+FileWritter,


  };

}
