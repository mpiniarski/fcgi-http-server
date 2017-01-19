#include "../../exceptions.h"

class FileReadingException : public Exception {
public:
    FileReadingException() : Exception("Could not read file") {}

};