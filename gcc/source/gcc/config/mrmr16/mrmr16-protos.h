/* Prototypes for mrmr16.c functions used in the md file & elsewhere.
   Copyright (C) 2009-2021 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

extern void  mrmr16_expand_prologue (void);
extern void  mrmr16_expand_epilogue (void);
extern int   mrmr16_initial_elimination_offset (int, int);
extern bool  mrmr16_offset_address_p (rtx);
extern void  mrmr16_split_symbolic_move (rtx, rtx);
extern void  mrmr16_push_emit (rtx, rtx, int);
extern void  mrmr16_pop_emit (rtx, rtx, int);