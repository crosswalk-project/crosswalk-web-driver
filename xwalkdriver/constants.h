#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include "../../../base/files/file_path.h"


#define FPL FILE_PATH_LITERAL
const char    kInitialProfile[] = "Default";
const base::FilePath::CharType kPreferencesFilename[] = FPL("Preferences");
const base::FilePath::CharType kLocalStateFilename[] = FPL("Local State");
const base::FilePath::CharType kFirstRunSentinel[] = FPL("First Run");

//const char    kInitialProfile[] = "Default";


#endif 
