//
//                   **** ViZiGO (C) 2000 Vincent Penne - Richard Christophe ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file pointer.h
 *
 * Pointer class definition
 *
 */

#ifndef POINTER_H
#define POINTER_H


class Pointer {
  

 public:
  int xpointer;
  int ypointer;
  int button;
  
  int size;
  
  
  Pointer();
  
  ~Pointer();

  void set_size(int s);
  inline int get_size() { return size; }
  void move();
  void draw();
  
};

#endif
