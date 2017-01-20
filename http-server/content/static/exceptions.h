#include "../../exceptions.h"

class FileReadingException : public Exception {
public:
    FileReadingException() : Exception("Could not read file") {}

};

class FileTypeRecognitionException : public Exception {
public:
    FileTypeRecognitionException() : Exception("Could not detect file type") {}
};
