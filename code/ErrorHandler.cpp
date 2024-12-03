#include "util.hpp"

// static
void ErrorHandler::PrintError(uint32_t error_code) {
    switch (error_code) {
    case RUNNING_NULL_SCENE:
        std::sprintf(stderr, "Error! Running a null scene.\n");
        break;
    case CHANGE_TO_NULL_SCENE:
        std::sprintf(stderr, "Error! No scene matches the tag.\n");
        break;
    default:
        std::sprintf(stderr, "Error! Undefined error code.\n");
    }
    exit(1);
}