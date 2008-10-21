//
//                   **** ViZiGO (C) 2000 Vincent Penne - DiSiGo  Richard Christophe ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file term.h
 *
 * Term class definition
 *
 */

#ifndef TERM_H
#define TERM_H

#define LINES 200

class Term {
  
  public:
	char lines[LINES][100];
	int lastline;

  
  Term();
  
  ~Term();

  void add(char* s);
  void drawlines();
  void draw();
  
};

#endif

