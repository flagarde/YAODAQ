/*
 * Copyright (c) 2018 Lagarde François lagarde.at.sjtu.edu.cn
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "SerialConnector.hpp"

#include "Exception.hpp"
#include "StatusCode.hpp"

#include <string>

namespace Serial
{
SerialConnector::SerialConnector(): Connector("Serial")
{
  m_ports = serial::list_ports();
}

void SerialConnector::DoConnect()
{
  if(!m_serial.isOpen()) { m_serial.open(); }
}

void SerialConnector::DoDisconnect()
{
  if(m_serial.isOpen()) m_serial.close();
}

void SerialConnector::verifyParameters()
{
  setPort();
  setBaudRate();
  setTimeout();
  setBytesize();
  setParity();
  setStopbits();
  setFlowcontrol();
}

void SerialConnector::setPort()
{
  try
  {
    m_port = toml::find<std::string>(getParameters(), "Port");
    m_serial.setPort(m_port);
  }
  catch(const std::out_of_range& e)
  {
    Exception(StatusCode::NOT_FOUND, "\"Port\" key not set !");
  }
}

void SerialConnector::setBaudRate()
{
  m_baudrate = toml::find_or<double>(getParameters(), "Baudrate", 9600);
  m_serial.setBaudrate(m_baudrate);
}

void SerialConnector::setTimeout()
{
  int timeout{0};
  try
  {
    timeout   = toml::find<int>(getParameters(), "Timeout");
    m_timeout = serial::Timeout::simpleTimeout(timeout);
  }
  catch(const std::out_of_range& e)
  {
  }
  m_serial.setTimeout(m_timeout);
}

void SerialConnector::setBytesize()
{
  int bytesize{0};
  try
  {
    bytesize = toml::find<int>(getParameters(), "Bytesize");
    if(bytesize == 8) m_bytesize = serial::bytesize_t::eightbits;
    else if(bytesize == 7)
      m_bytesize = serial::bytesize_t::sevenbits;
    else if(bytesize == 6)
      m_bytesize = serial::bytesize_t::sixbits;
    else if(bytesize == 5)
      m_bytesize = serial::bytesize_t::fivebits;
    else
      Exception(StatusCode::NOT_FOUND, "\"Bytesize\" should be 8 7 6 or 5 !");
  }
  catch(const std::out_of_range& e)
  {
  }
  m_serial.setBytesize(m_bytesize);
}

void SerialConnector::setParity()
{
  try
  {
    std::string parity = toml::find<std::string>(getParameters(), "Parity");
    if(parity == "none") m_parity = serial::parity_t::parity_none;
    else if(parity == "odd")
      m_parity = serial::parity_t::parity_odd;
    else if(parity == "even")
      m_parity = serial::parity_t::parity_even;
    else if(parity == "mark")
      m_parity = serial::parity_t::parity_mark;
    else if(parity == "space")
      m_parity = serial::parity_t::parity_space;
    else
      Exception(StatusCode::NOT_FOUND, "\"Parity\" should be none, odd, even, mark or space !");
  }
  catch(const std::out_of_range& e)
  {
  }
  m_serial.setParity(m_parity);
}

void SerialConnector::setStopbits()
{
  try
  {
    double stop = toml::find<double>(getParameters(), "Stopbits");
    if(stop == 1) m_stopbits = serial::stopbits_t::stopbits_one;
    else if(stop == 2)
      m_stopbits = serial::stopbits_t::stopbits_two;
    else if(stop == 1.5)
      m_stopbits = serial::stopbits_t::stopbits_one_point_five;
    else
      Exception(StatusCode::NOT_FOUND, "\"Stopbits\" should be 1, 1.5 or 2 !");
  }
  catch(const std::out_of_range& e)
  {
  }
  m_serial.setStopbits(m_stopbits);
}

void SerialConnector::setFlowcontrol()
{
  try
  {
    std::string flow = toml::find<std::string>(getParameters(), "Flowcontrol");
    if(flow == "none") m_flowcontrol = serial::flowcontrol_t::flowcontrol_none;
    else if(flow == "software")
      m_flowcontrol = serial::flowcontrol_t::flowcontrol_software;
    else if(flow == "hardware")
      m_flowcontrol = serial::flowcontrol_t::flowcontrol_hardware;
    else
      Exception(StatusCode::NOT_FOUND, "\"Flowcontrol\" should be none, software or hardware !");
  }
  catch(const std::out_of_range& e)
  {
  }
  m_serial.setFlowcontrol(m_flowcontrol);
}

Response SerialConnector::sendCommand(const Command& command) const
{
  /* std::string respond="";
    std::string real_command=command[0].String()+"\n";
    std::size_t bytes_wrote = m_serial.write(real_command);
    if(bytes_wrote==real_command.size()) 
    {
      respond=m_serial.read(bytes_wrote+2000);
      if(respond.size()==bytes_wrote) return std::string();
      respond=respond.erase(0,bytes_wrote);
    }
    return Value(respond);*/
}

}  // namespace Serial
