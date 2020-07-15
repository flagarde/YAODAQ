 #pragma once
 #include <memory>
 
 #include "Board.hpp"
 #include "File.hpp"
 
 class FileWritter : public Module 
 {
 public:
   FileWritter(const std::string& name = "", const std::string& type = "FileWritter");
   void setFile(std::unique_ptr<File>&& file);
   void setFileName(const std::string& name);
   void setFileOptions(const std::string& option);
   virtual ~FileWritter()=default;
 protected:
   void DoOnData(const Data& data);
   void DoStart();
   void DoStop();
 private:
   std::unique_ptr<File> m_File{nullptr};
 };
