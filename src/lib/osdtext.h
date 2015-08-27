#ifndef CSD462_TEXTOSD_H
#define CSD462_TEXTOSD_H

#include <string>
#include <vector>

#include "glew.h"

#include "color.h"

// forward declare freetype stuff 
struct FT_LibraryRec_; 
typedef struct FT_LibraryRec_* FT_Library; 
struct FT_FaceRec_; 
typedef struct FT_FaceRec_*  FT_Face;

namespace CSD462 {

// embeded font
extern "C" const char   osdfont[];
extern "C" const size_t osdfont_size;

struct OSDLine {
  
  // UID of the line
  int id;

  // screen space anchor position
  float x, y;

  // line content
  std::string text; 

  // font size
  size_t size;

  // font color
  Color color;
  
};

/**
 * Provides routines for managing on-screen display text. 
 * Note that this requires GL_BLEND enabled to work. Do note this is a very
 * basic implementation and the cost of all operations increases linearly with 
 * respect to the number of lines and the length of the lines. 
 */
class OSDText {
 public:
  
  /**
   * Constructor.
   * Creates an empty text OSD.
   */
  OSDText();

  /**
   * Destructor.
   * Destroys the textOSD instance and free resources.
   */
  ~OSDText();

  /**
   * Initializes resources required for rendering text.
   * This will load a freetype font and compile shaders, etc.
   * \return 0 if successful, -1 on error.
   */
  int init();

  /**
   * Draw the text OSD.
   * Render all the lines.
   */
  void render();

  /**
   * Resize internal scales when context size has changed.
   * This should be called on a window resize, etc.
   */
  void resize(size_t w, size_t h);

  /**
   * Add a line of text to the OSD.
   * This uses GL's screen space standard and maps the horizontal space
   * to [-1, 1] from left to right and the vertical space to [-1, 1] from
   * bottom to top.
   * \param x Horizontal coordinate of the anchor. 
   * \param y Vertical coordinate of the anchor.
   * \param text The text to add.
   * \param size The font size of the text.
   * \param color The color of the text.
   * \return the line index if successfully added. A valid line index
   *         is non-negative. If the line was not successfully added, 
   *         -1 is returned.
   */
  int add_line(float x, float y, std::string text = "",
               size_t size = 16, Color color = Color::White);

  /**
   * Deletes a line.
   * If the given id is not valid, the call has no effect.
   * \param line_id Index of the line to be removed. 
   */
  void del_line(int line_id);

  /**
   * Set the anchor position of a given line.
   * If the given id is not valid, the call has no effect.
   * \param line_id Index of the line to set the text.
   * \param x The new x coordinate to set for the line.
   * \param y The new y coordinate to set for the line.
   */
  void set_anchor(int line_id, float x, float y);

  /**
   * Set the text of a given line.
   * If the given id is not valid, the call has no effect.
   * \param line_id Index of the line to set the text.
   * \param text The new text to set for the line.
   */
  void set_text(int line_id, std::string text);

  /**
   * Set the font size of a given line.
   * If the given id is not valid, the call has no effect.
   * \param line_id Index of the line to set the text.
   * \param size The new size to set for the line.
   */
  void set_size(int line_id, size_t size);

  /**
   * Set the font color of a given line.
   * If the given id is not valid, the call has no effect.
   * \param line_id Index of the line to set the text.
   * \param color The new color to set for the line.
   */
  void set_color(int line_id, Color color);

 private:

  // draw a single line
  void draw_line(OSDLine line);

  // internal scale factors
  float sx, sy;

  // line id counter
  int next_id;

  // freetype font
  FT_Library* ft; FT_Face* face;

  // lines to draw
  std::vector<OSDLine> lines;

  // GL stuff
  GLuint vbo;
  GLuint program;
  GLint attribute_coord;
  GLint uniform_tex;
  GLint uniform_color;
  
  // GL helpers
  GLuint compile_shaders();
  GLint  get_attribu(GLuint program, const char *name);
  GLint  get_uniform(GLuint program, const char *name);
  
}; // class textOSD

} // namespace CSD462

#endif // CSD462_TEXTOSD_H
