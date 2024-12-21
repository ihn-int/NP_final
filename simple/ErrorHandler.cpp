#include "Scene.hpp"

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
        break;
    case FONT_NOT_FOUND:
        std::fprintf(stderr, "Error! Required font not found.\n");
        break;
    case IMAGE_NOT_FOUND:
        std::fprintf(stderr, "Error! Required image not found.\n");
        break;
    case INVALID_USAGE:
        std::fprintf(stderr, "Error! Wrong arguments.\n");
        break;
    case UNKNOWN_OPCODE:
        std::fprintf(stderr, "Error! Unknown instruction from server.\n");
        break;
    default:
        std::fprintf(stderr, "Error! Undefined error code: %d.\n", error_code);
    }
    exit(error_code);
}