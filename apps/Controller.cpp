#include "Controller.hpp"

#include "CLI/CLI.hpp"
#include "ProgramInfos.hpp"
#include "magic_enum.hpp"

#include "linenoise.hpp"

#include <iostream>
#include <future>
#include <any>

using namespace yaodaq;




int main(int argc, char** argv)
{

  std::string path = ".history.txt";

  // Enable the multi-line mode
  linenoise::SetMultiLine(true);

  // Set max length of the history
  linenoise::SetHistoryMaxLen(static_cast<int>(std::numeric_limits<int>::max()));

  // Load history
  linenoise::LoadHistory(path.c_str());




  ProgramInfos infos;
  infos.Logo();
  CLI::App  app{"Controller"};
  int port{GeneralParameters::getPort()};
  app.add_option("-p,--port", port, "Port to listen")->check(CLI::Range(0, 65535));
  std::string host{GeneralParameters::getHost()};
  app.add_option("-i,--ip", host, "IP of the server")->check(CLI::ValidIPV4);
  std::string name {"Controller"};
  app.add_option("-n,--name", name, "Name of the Controller");
  std::string verbosity{"trace"};
  app.add_option("-v,--verbosity", verbosity, "Verbosity")->check(
    [](const std::string& t) {
      if(t != "off" && t != "trace" && t != "info" && t != "debug" && t != "warning" && t != "critical") return "Wrong verbosity level";
                                                                      else
                                                                        return "";
    },
    "Verbosity level", "Verbosity level");
  try
  {
    app.parse(argc, argv);
  }
  catch(const CLI::ParseError& e)
  {
    spdlog::error("{}", e.what());
    return app.exit(e);
  }

  GeneralParameters::setPort(port);
  GeneralParameters::setHost(host);

  Controller controller(name);
  controller.setVerbosity(verbosity);

  //controller.startListening();
  CLI::App  options{"Options"};
  std::string action;
  options.add_option("-a,--action", action, "Action");
  std::string commandName;
  options.add_option("-c,--command", commandName, "Command");
  std::vector<std::string> params;
  options.add_option<std::vector<std::string>>("--params", params, "Parameters");


  std::string log;
  options.add_option("-l,--log", log, "log to send.");
  std::string level{"Info"};
  options.add_option("--level", level, "level of the log.");
  bool quit{false};
  options.add_flag("--quit",quit,"Quit the application.");
  //jsonrpc::Request::Parameters parameters;

  std::string line;
  std::thread loop(&Controller::loop,&controller);
  loop.detach();
  controller.wait();
  do
  {

    if(linenoise::Readline(fmt::format("{}{}{}{}",fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"["),fmt::format(fg(fmt::color::green) | fmt::emphasis::bold,"要DAQ "),name,fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold," ] ")).c_str(),line))
    {
      break;
    }
    try
    {
      options.parse(line);
      if(quit)
      {
        controller.stopListening();
        std::exit(0);
      }
      if(!action.empty())
      {
        controller.sendAction(action);
      }
      if(!commandName.empty())
      {

        std::vector<nlohmann::json> parameters;
        for(std::size_t i=0;i!=params.size();++i)
        {
          try
          {
            int value = std::stoi(params[i]);
            parameters.push_back(nlohmann::json(value));
          }
          catch(const std::invalid_argument& e)
          {
            try
            {
              int value = std::stod(params[i]);
              parameters.push_back(nlohmann::json(value));
            }
            catch(const std::invalid_argument& e)
            {
              parameters.push_back(nlohmann::json(params[i]));
            }
          }
          std::cout<<params[i]<<std::endl;

        }
        try
        {
          std::pair<std::map<Identifier,nlohmann::json>,std::map<Identifier,nlohmann::json>> responses = controller.CallMethod<nlohmann::json>("FFF",commandName,parameters);
          parameters.clear();
          params.clear();
          fmt::print(fg(fmt::color::green),"Results :\n");
          for(std::map<Identifier,nlohmann::json>::iterator it=responses.first.begin(); it!=responses.first.end();++it)
          {
            if(it->second.is_null()) fmt::print("{} From {} : {} \n",fmt::format(fg(fmt::color::green),"* "),fmt::format(fmt::emphasis::bold,it->first.get()),"");
            fmt::print("{} From {} : {} \n",fmt::format(fg(fmt::color::green),"* "),fmt::format(fmt::emphasis::bold,it->first.get()),it->second.dump());
          }
          fmt::print(fg(fmt::color::red),"Errors :\n");
          for(std::map<Identifier,nlohmann::json>::iterator it=responses.second.begin(); it!=responses.second.end();++it)
          {
            std::string what;
            if(it->second.contains("data")) what = fmt::format("   code : {}, message : \"{}\", data : {}\n",it->second["code"].get<int>(),it->second["message"].get<std::string>(),it->second["data"].dump());
            else what = fmt::format("   code : {}, message : \"{}\"\n",it->second["code"].get<int>(),it->second["message"].get<std::string>());
            if(it->second["code"].get<int>()==-32601) fmt::print(fg(fmt::color::orange),"X From {} :\n{}",fmt::format(fmt::emphasis::bold,it->first.get()),fmt::format(fg(fmt::color::orange),"{}",what));
            else fmt::print(fg(fmt::color::red),"X From {} :\n{}",fmt::format(fmt::emphasis::bold,it->first.get()),fmt::format(fg(fmt::color::red),"{}",what));
          }
        }
        catch(...)
        {
          controller.error("Error processing the Responses!");
        }


        //Command command = controller.command(commandName,parameters);
        //controller.send(command);
      }
      if(!log.empty())
      {
        auto _level = magic_enum::enum_cast<yaodaq::LEVEL>(level);
        if(_level.has_value())
        {
          controller.log(_level.value(),log);
        }
        else
        {
          fmt::print("{}{}{}{}\n",fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"["),fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"要DAQ"),fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"] "),fmt::format("{} level is unknown ! Falling back to Info",level));
          controller.log(LEVEL::Info,log);
        }
      }
    }
    catch(const CLI::ParseError& e)
    {
      fmt::print("{}{}{}{}\n",fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"["),fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"要DAQ"),fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"] "),e.what());
      options.exit(e);
    }
    catch(const Exception& e)
    {
      fmt::print("{}{}{}{}\n",fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"["),fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,"要DAQ"),fmt::format(fg(fmt::color::orange) | fmt::emphasis::bold,"] "),e.what());
    }
    action.clear();
    commandName.clear();
    log.clear();
    // Add line to history
    linenoise::AddHistory(line.c_str());

    // Save history
    linenoise::SaveHistory(path.c_str());
  }
  while(true);



}
