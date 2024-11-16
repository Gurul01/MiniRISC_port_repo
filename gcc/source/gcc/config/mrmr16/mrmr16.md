;; Machine description for mrmr16
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
;; mrmr16 specific constraints, predicates and attributes
;; -------------------------------------------------------------------------

(include "constraints.md")
(include "predicates.md")

; Most instructions are two bytes long.
(define_attr "length" "" (const_int 2))

;; -------------------------------------------------------------------------
;; nop instruction
;; -------------------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "")

;; -------------------------------------------------------------------------
;; Arithmetic instructions
;; -------------------------------------------------------------------------

(define_insn "addhi3"
    [(set (match_operand:HI 0 "register_operand" "=r,r")
	  (plus:HI
	   (match_operand:HI 1 "register_operand" "0,r")
	   (match_operand:HI 2 "mrmr16_add_sub_inc_dec_operand" "k,r")))]
           ""
           "@
           inc\\t%0
           add\\t%0 %1 %2"
	   [])

(define_insn "subhi3"
    [(set (match_operand:HI 0 "register_operand" "=r,r")
	  (minus:HI
	   (match_operand:HI 1 "register_operand" "0,r")
	   (match_operand:HI 2 "mrmr16_add_sub_inc_dec_operand" "k,r")))]
           ""
           "@
           dec\\t%0
           sub\\t%0 %1 %2"
	   [])

(define_insn "mulhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (mult:HI
	   (match_operand:HI 1 "register_operand" "r")
	   (match_operand:HI 2 "register_operand" "r")))]
	   ""
	   "mul\\t%0 %1 %2")

(define_insn "divhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (div:HI
	   (match_operand:HI 1 "register_operand" "r")
	   (match_operand:HI 2 "register_operand" "r")))]
	   ""
	   "div\\t%0 %1 %2")

(define_insn "ashlhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (ashift:HI
	   (match_operand:HI 1 "register_operand" "r")
	   (match_operand:HI 2 "register_operand" "r")))]
           ""
           "sll\\t%0 %1 %2")

(define_insn "ashrhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (ashiftrt:HI
	   (match_operand:HI 1 "register_operand" "r")
	   (match_operand:HI 2 "register_operand" "r")))]
           ""
           "srl\\t%0 %1 %2")

;; -------------------------------------------------------------------------
;; Unary arithmetic instructions
;; -------------------------------------------------------------------------

(define_insn "one_cmplhi2"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (not:HI (match_operand:HI 1 "register_operand" "r")))]
          ""
          "not\\t%0 %1")

;; -------------------------------------------------------------------------
;; Logical operators
;; -------------------------------------------------------------------------

(define_insn "andhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (and:HI (match_operand:HI 1 "register_operand" "r")
		  (match_operand:HI 2 "register_operand" "r")))]
                  ""
                  {
                  return "and\\t%0 %1 %2";
                  })

(define_insn "xorhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (xor:HI (match_operand:HI 1 "register_operand" "r")
		  (match_operand:HI 2 "register_operand" "r")))]
                  ""
                  {
                  return "xor\\t%0 %1 %2";
                  })

(define_insn "iorhi3"
    [(set (match_operand:HI 0 "register_operand" "=r")
	  (ior:HI (match_operand:HI 1 "register_operand" "r")
		  (match_operand:HI 2 "register_operand" "r")))]
                  ""
                  {
                  return "or\\t%0 %1 %2";
                  })

;; -------------------------------------------------------------------------
;; Move instructions
;; -------------------------------------------------------------------------

(define_insn "movhi_push"
    [(set (mem:HI (pre_dec:HI (reg:HI 1)))
  	  (match_operand:HI 0 "register_operand" "r"))]
          ""
          "push\\t%0")

(define_insn "movhi_pop"
    [(set (match_operand:HI 1 "register_operand" "=r")
  	  (mem:HI (post_inc:HI (match_operand:HI 0 "register_operand" "r"))))]
          ""
          "pop\\t%1")

(define_expand "movhi"
    [(set (match_operand:HI 0 "nonimmediate_operand" "")
	  (match_operand:HI 1 "mrmr16_general_movsrc_operand" ""))]
	  ""
	  "")

(define_insn "movhi_internal"
    [(set (match_operand:HI 0 "nonimmediate_operand" "=r,r,r,r,W")
          (match_operand:HI 1 "mrmr16_general_movsrc_operand" "r,i,Y,W,r"))]
          ""
          "@
          mov\\t%0 %1
          ldi\\t%0 %1
          ldi\\t%0 %1
          ld\\t%0 %1
          st\\t%0 %1"
          [(set_attr "length" "2,4,4,2,2")])

(define_split
    [(set (match_operand:HI 0 "nonimmediate_operand" "")
	  (match_operand:HI 1 "mrmr16_general_movsrc_operand" ""))]
	  "
          ((GET_CODE (operands[0]) == MEM) && (GET_CODE (XEXP (operands[0], 0)) == SYMBOL_REF)) ||
          ((GET_CODE (operands[1]) == MEM) && (GET_CODE (XEXP (operands[1], 0)) == SYMBOL_REF)) ||
          ((GET_CODE (operands[0]) == MEM) && (GET_CODE (XEXP (operands[0], 0)) == CONST)) ||
          ((GET_CODE (operands[1]) == MEM) && (GET_CODE (XEXP (operands[1], 0)) == CONST))
          "
	  [(const_int 0)]
	  {
	    mrmr16_split_symbolic_move (operands[0], operands[1]);
	    DONE;
	  })

;; -------------------------------------------------------------------------
;; Call and Jump instructions
;; -------------------------------------------------------------------------

(define_expand "call"
    [(call (match_operand:HI 0 "memory_operand" "")
	   (match_operand 1 "general_operand" ""))]
           ""
           {
           gcc_assert (MEM_P (operands[0]));
           })

(define_insn "*call"
    [(call (mem:HI (match_operand:HI
		    0 "nonmemory_operand" "i,r"))
	   (match_operand 1 "" ""))]
           ""
           "@
           call\\t%0
           callr\\t%0"
           [(set_attr "length"	"4,2")])

(define_insn "indirect_jump"
    [(set (pc) (match_operand:HI 0 "nonimmediate_operand" "r"))]
    ""
    "jmpr\\t%0")

(define_insn "jump"
    [(set (pc) (label_ref (match_operand 0 "" "")))]
    ""
    "jmp\\t%l0")

;; -------------------------------------------------------------------------
;; Compare instructions
;; -------------------------------------------------------------------------

(define_constants
  [(CC_REG 11)])

(define_expand "cbranchhi4"
  [(set (reg:CC CC_REG)
        (compare:CC
         (match_operand:HI 1 "general_operand" "")
         (match_operand:HI 2 "general_operand" "")))
   (set (pc)
        (if_then_else (match_operator 0 "comparison_operator"
                       [(reg:CC CC_REG) (const_int 0)])
                      (label_ref (match_operand 3 "" ""))
                      (pc)))]
  ""
  "
  /* Force the compare operands into registers.  */
  if (GET_CODE (operands[1]) != REG)
	operands[1] = force_reg (HImode, operands[1]);
  if (GET_CODE (operands[2]) != REG)
	operands[2] = force_reg (HImode, operands[2]);
  ")

(define_insn "*cmphi"
  [(set (reg:CC CC_REG)
	(compare
	 (match_operand:HI 0 "register_operand" "r")
	 (match_operand:HI 1 "register_operand"	"r")))]
  ""
  "cmp\\t%0 %1")

;; -------------------------------------------------------------------------
;; Branch instructions
;; -------------------------------------------------------------------------

(define_code_iterator cond [ne eq lt ltu gt gtu ge le geu leu])
(define_code_attr CC [(ne "ne") (eq "eq") (lt "lt") (ltu "ltu")
		      (gt "gt") (gtu "gtu") (ge "ge") (le "le")
		      (geu "geu") (leu "leu") ])

(define_insn "*b<cond:code>"
  [(set (pc)
	(if_then_else (cond (reg:CC CC_REG)
			    (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
{
  return "b<CC>\\t%l0";
})

;; -------------------------------------------------------------------------
;; Prologue & Epilogue
;; -------------------------------------------------------------------------

(define_expand "prologue"
    [(clobber (const_int 0))]
    ""
    "
{
  mrmr16_expand_prologue ();
  DONE;
}
")

(define_expand "epilogue"
    [(return)]
    ""
    "
{
  mrmr16_expand_epilogue ();
  DONE;
}
")

(define_insn "ret"
    [(return)]
    "reload_completed"
    "ret")

;; -------------------------------------------------------------------------
;; Interrupt related instructions
;; -------------------------------------------------------------------------

(define_c_enum "unspecv" [
  UNSPECV_IRET
  UNSPECV_STI
  UNSPECV_CLI
  ])

(define_insn "iret"
    [(return)
    (unspec_volatile [(const_int 0)] UNSPECV_IRET)]
    ""
    "iret")

(define_insn "sti"
    [(unspec_volatile [(const_int 0)] UNSPECV_STI)]
    ""
    "sti")

(define_insn "cli"
    [(unspec_volatile [(const_int 0)] UNSPECV_CLI)]
    ""
    "cli")

;; Local Variables:
;; mode: lisp
;; indent-tabs-mode: t
;; End:
