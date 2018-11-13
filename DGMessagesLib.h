#ifndef DGMESSAGESLIB_H_INCLUDED
#define DGMESSAGESLIB_H_INCLUDED
#include <string>
namespace message
{
	/*Function create header of DG's data files*/
	std::string headerFile();

	/*Function display undefined keyWord error*/
	std::string undfKeyW(std::string keyW, std::string location);

	/*Function display opening file error*/
	std::string opFError(std::string fileName, std::string location);

	/*Function writes logFile to report error to user*/
	void writeLog(std::string location, std::string caseName, std::string str);

	/*Functions gets time data from system*/
	std::string getTime();

	/*Function create header of files p T U*/
	std::string headerpTU(std::string file);
}
#endif // DGMESSAGESLIB_H_INCLUDED
