;; Predicate definitions for minirisc
;; Copyright (C) 2009-2022 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; -------------------------------------------------------------------------
;; Predicates
;; -------------------------------------------------------------------------

;; Nonzero if OP can be source of a simple move operation.
(define_predicate "minirisc_general_movsrc_operand"
  (match_code "mem,const_int,reg,subreg,symbol_ref,const")
{
  if (MEM_P (op) && GET_CODE (XEXP (op, 0)) == SYMBOL_REF)
    return 1;

  if (MEM_P (op) && GET_CODE (XEXP (op, 0)) == REG)
    return 1;

  return general_operand (op, mode);
})

;; Nonzero if OP can be an operand to an add/sub instruction.
(define_predicate "minirisc_add_sub_operand"
  (ior (match_code "reg")
       (and (match_code "const_int")
	    (match_test "IN_RANGE (INTVAL (op), 0, 255)"))))

;; Nonzero if OP can be an operand to a minirisc_logic_operand instruction.
(define_predicate "minirisc_logic_operand"
  (ior (match_code "reg")
       (and (match_code "const_int")
	    (match_test "IN_RANGE (INTVAL (op), -128, 255)"))))

(define_predicate "minirisc_add_sub_inc_dec_operand"
    (ior (match_code "reg")
	 (and (match_code "const_int")
	      (match_test "INTVAL (op) == 1"))))

(define_predicate "minirisc_comparison_operator"
    (match_code "eq"))

;; Local Variables:
;; mode: lisp
;; indent-tabs-mode: t
;; End:
