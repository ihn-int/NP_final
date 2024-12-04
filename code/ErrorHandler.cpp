#include "util.hpp"

// static
void ErrorHandler::printError(uint32_t error_code) {
    switch (error_code) {
    case RUNNING_NULL_SCENE:
        std::fprintf(stderr, "Error! Running a null scene.\n");
        break;
    case CHANGE_TO_NULL_SCENE:
        std::fprintf(stderr, "Error! No scene matches the name.\n");
        break;
    case DELETE_NULL_GAMEOBJECT:
        std::fprintf(stderr, "Error! Delete a null game object.\n");
    default:
        std::fprintf(stderr, "Error! Undefined error code.\n");
    }
    exit(1);
}