#include "engine/fonts.h"

#include "engine/assertions.h"
#include "engine/log.h"
#include "engine/texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

FT_Library s_ftLibrary;

int initFontLibrary()
{
    FT_Error error = FT_Init_FreeType(&s_ftLibrary);
    if (error) {
        LOG_ERR("[Font] Unable to load the font library (FT_Error %d)", error);
        return error;
    }

    LOG_INFO("[Font] Font library successfully loaded");
    return error;
}

int destroyFontLibrary()
{
    FT_Error error = FT_Done_FreeType(s_ftLibrary);
    if (error) {
        LOG_ERR("[Font] Unable to destroy the font library (FT_Error %d)", error);
        return error;
    }

    LOG_INFO("[Font] Font library successfully destroyed");

    return error;
}

// Assumes 1 Pixel == 1 Byte
Font* loadFontFromFile(const char* _path, int _fontSizeInPx, int _charStart, int _charEnd)
{
    FT_Face face;
    FT_Error error;

    error = FT_New_Face(s_ftLibrary, _path, 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        ASSERT_MSG(FALSE, "[Font] Unsupported font format");
        LOG_ERR("[Font] Unsupported font format in %s (FT_Error %d)", _path, error);
        return NULL;
    } else if (error != 0) {
        ASSERT_MSG(FALSE, "[Font] Unable to load font face");
        LOG_ERR("[Font] Unable to load font face for %s", _path);
        return NULL;
    }

    error = FT_Set_Pixel_Sizes(face, 0, _fontSizeInPx);
    if (error != 0) {
        ASSERT_MSG(FALSE, "[Font] Unable to set the font size");
        LOG_ERR(FALSE, "[Font] Unable to set the font size to %d", _fontSizeInPx);
        return NULL;
    }

    const int nbChars = _charEnd - _charStart + 1; // +1 because 'a' - 'a' = 0
    if (nbChars <= 0) {
        ASSERT_MSG(FALSE, "[Font] Invalid range of characters");
        LOG_ERR(FALSE, "[Font] Invalid range of characters (%d to %d)", _charStart, _charEnd);
        return NULL;
    }

    // Buffer size is arbitrary (square and big enough)
    const size_t bufferWidth = 512;
    const size_t bufferHeight = 512;
    const size_t bufferSize = bufferWidth * bufferHeight;

    uint8* bitmapBuffer = malloc(bufferSize);
    if (bitmapBuffer == NULL) {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d bytes", bufferSize);
        return NULL;
    }
    memset(bitmapBuffer, 0, bufferSize);

    Font* font = malloc(sizeof(Font));
    if (font == NULL) {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d Bytes", sizeof(Font));
        return NULL;
    }
    memset(font, 0, sizeof(Font));

    font->glyphs = malloc(nbChars * sizeof(FontGlyph));
    if (font->glyphs == NULL) {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d Bytes", sizeof(FontGlyph));
        return NULL;
    }
    memset(font->glyphs, 0, sizeof(font->glyphs));

    int currentX = 0;
    int currentY = 0;
    int gap = 5; // Arbitrary gap between letters

    for (int codepoint = _charStart; codepoint <= _charEnd; ++codepoint) {
        FT_UInt glyph_index = FT_Get_Char_Index(face, codepoint);
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        int glyphWidth = face->glyph->bitmap.width;
        int glyphHeight = face->glyph->bitmap.rows;
        ASSERT_MSG(glyphWidth != 0, "FT_Render_Glyph generated a bitmap invalid width=0");
        ASSERT_MSG(glyphHeight != 0, "FT_Render_Glyph generated a bitmap invalid height=0");

        if (currentX + glyphWidth >= bufferWidth) {
            currentX = 0;
            currentY += _fontSizeInPx + gap;
        }

        if (currentY + glyphHeight > bufferHeight) {
            // This means the buffer size is too small!
            LOG_ERR("[Font] The buffer size is to small to have all the requested glyphs");
            ASSERT_MSG(FALSE, "[Font] The buffer size is to small to have all the requested glyphs");
            break; // In release, if this appears, load glyphs until now
        }

        // TODO this does not take the gap into account (use a small gap until it is fixed)
        font->glyphs[codepoint - _charStart].uvX0 = currentX / bufferWidth;
        font->glyphs[codepoint - _charStart].uvX1 = (currentX + glyphWidth) / bufferWidth;

        font->glyphs[codepoint - _charStart].uvY0 = currentY / bufferHeight;
        font->glyphs[codepoint - _charStart].uvY1 = (currentY + glyphHeight) / bufferHeight;

        font->glyphs[codepoint - _charStart].width = glyphWidth;
        font->glyphs[codepoint - _charStart].height = glyphHeight;

        LOG_DBG("'%c'\tw=%d\th=%d\tX=%d\tY=%d", codepoint, glyphWidth, glyphHeight, currentX, currentY);

        for (int rowY = 0; rowY < face->glyph->bitmap.rows; ++rowY) {
            const void* bufferRowSrc = face->glyph->bitmap.buffer + (glyphWidth * rowY);
            void* bufferRowDst = bitmapBuffer + currentX + (currentY * bufferWidth) + (bufferWidth * rowY);

            memcpy(bufferRowDst, bufferRowSrc, glyphWidth);
        }

        currentX += glyphWidth + gap;
    }

    // DEBUG: uncomment to create a debug png with the generated glyphs bitmap
    // stbi_write_png("font_generated_bitmap.png", bufferWidth, bufferHeight, 1, bitmapBuffer, 0);

    Texture texture = makeTexture(bitmapBuffer, bufferWidth, bufferHeight, 1);

    font->sizeInPx = _fontSizeInPx;
    font->charStart = _charStart;
    font->charEnd = _charEnd;
    font->textureID = texture.id;

    free(bitmapBuffer);
    FT_Done_Face(face);

    return font;
}

void destroyFont(Font* _font)
{
    ASSERT_MSG(_font != NULL, "[Font] Invalid parameter");

    if (_font != NULL) {
        free(_font->glyphs);
        free(_font);
    }
}
