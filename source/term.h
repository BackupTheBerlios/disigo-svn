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


class Term {
  
  public:
	char lines[200][100];
	int lastline;
	

  
  Term();
  
  ~Term();

  void add(char* s);
  void draw();
  
};

#endif

