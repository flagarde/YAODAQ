#include "Configuration.hpp"
#include <iostream>
int main()
{
 
  
  /*toml::value data = toml::parse("./toto.cfg");
  // loop over all the `[[robot]]` defined in a file
  for(const auto& robot : toml::find<toml::array>(data, "robot"))
  {
    // loop over [[robot.segment]] defined in the n-th `[[robot]]`
    for(const auto& segment : toml::find<toml::array>(robot, "segment"))
    {
      std::cout << "name = " << toml::find<std::string>(segment, "name") << std::endl;
    }
  }*/
  
  Configuration toto;
  toto.setFileName("./toto.cfg");
  toto.parse();
  
}
