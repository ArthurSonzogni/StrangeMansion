#include <exception>
#include <iostream>

class Error : public std::exception
{
    public:
        Error(int _line, const char * _file, const std::string& _what) throw():
            line(_line), file(_file), w(_what) {}
        Error(const std::string& _what) throw() :
            line(-1), file("no specified"), w(_what) {}
        virtual const char* what() const throw()
        {
            std::cerr<<"Error:"<<std::endl;
            std::cerr<<"    File:"<<file<<std::endl;
            std::cerr<<"    Line:"<<line<<std::endl;
            std::cerr<<"    What:"<<w<<std::endl;
            return "";
        }
        virtual ~Error() throw() {}

    private:
        int line;
        const char * file;
        std::string w;
};
