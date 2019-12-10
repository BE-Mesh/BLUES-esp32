/*
 * __status.h
 * Set of possible return status from a be-mesh function
 */

#pragma once

namespace bemesh{
  // Standardized return code for bemesh functions.
  // If a new return code has to be added, please
  // refer to this list, and eventually update it
  // accordingly

  // Errors shall be identifies by negative values
  // while warning or success values shall be
  // positive.
  enum ErrStatus{
    //TODO
    NoCallbackError=-2,
    BufferFullError=-1,
    // Only warnings or succesful status below-------
    Success=0,
    //TODO
  };
}