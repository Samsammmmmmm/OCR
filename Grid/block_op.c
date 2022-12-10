#include "block_op.h"

void crop_borders(SDL_Surface *block)
{
  
}


SDL_Surface* block_resize(SDL_Surface *block, int pix_w, int pix_h)
{
  // Create a new surface with the desired dimensions
  SDL_Surface* wanted_surf_dim = SDL_CreateRGBSurface(0, pix_w, pix_h, 32, 0, 0, 0, 0);
  
  // Copy the pixels from the original surface and scale them to the new size
  SDL_BlitScaled(block, NULL, wanted_surf_dim, NULL);

  return block;
}
