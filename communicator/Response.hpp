#ifndef Response_hpp
#define Response_hpp

#include <string>

class Response
{
		private:
			std::string	m_responseMessage;

		public:
			Response();
			~Response();

			// void checkUri(std::string& methodType, std::string& uri);
};
#endif //Response_hpp
