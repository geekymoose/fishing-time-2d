#include "engine/fonts.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "engine/log.h"


FT_Library s_ftLibrary;


int initFontLibrary()
{
    FT_Error error = FT_Init_FreeType(&s_ftLibrary);
    if(error)
    {
        LOG_ERR("[Fonts] Unable to load the font library (FT_Error %d)", error);
        return error;
    }

    LOG_INFO("[Fonts] Font library successfully loaded");
    return 0;
}

Font makeFont(const char * path)
{
    ASSERT_MSG(path != NULL, "[Fonts] Invalid parameter (Not NULL expected)");

    FT_Face face;

    FT_Error error = FT_New_Face(s_ftLibrary, path, 0, &face);
    if(error == FT_Err_Unknown_File_Format)
    {
        LOG_ERR("[Fonts] Unsupported font format in %s (FT_Error %d)", path, error);
        //return error;
    }
    else if(error)
    {
        LOG_ERR("[Fonts] Unable to load font %s (FT_Error %d)", path, error);
        //return error;
    }

    error = FT_Set_Pixel_Sizes(face, 0, 16);
    if(error)
    {
        LOG_ERR("[Fonts] Unable to set font size for %s (FT_Error %d)", path, error);
        //return error;
    }
}
