//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file connection.h
 *
 * Connection class definition
 *
 */

#include "term.h"

#ifndef CONNECTION_H
#define CONNECTION_H

/** Class connection 
 *
 * Abstraction of a connection to a socket over internet 
 *
 * @warning should hide the file descritors, and instead provide some
 * read and write methods and a "install FLTK callback" method
 *
 */
class Connection {

  int sock;

 public:
  int fdin, fdout; ///< the two read and write file descriptors

  Connection();
  ~Connection();

/** open a connection
 *
 * @param a address
 * @param p port number
 * @return 1 on succes
 *
 */
  bool open( char* a, int p, Term* t );

/** close a connection */
  void close();

/** read n bytes from the socket */
  int read(void* buffer, int size);

/** write n bytes to the socket */
  int write(const void* buffer, int size);
};

#endif
