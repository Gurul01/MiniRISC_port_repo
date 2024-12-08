;; Machine description for minirisc
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
;; minirisc specific constraints, predicates and attributes
;; -------------------------------------------------------------------------

(include "constraints.md")
(include "predicates.md")

; Most instructions are two bytes long.
(define_attr "length" "" (const_int 1))




;; CONSTANT
(define_constants
  [(CC_REG 11)])


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

(define_insn "addqi3"
    [(set (match_operand:QI 0 "register_operand" "=r,r")
	  (plus:QI
	   (match_operand:QI 1 "register_operand" "0,0")
	   (match_operand:QI 2 "minirisc_add_operand" "r,i")))]
           ""
           "@
           ADD\\t%0 %2
           ADD\\t%0 %2"
	   [])

(define_insn "subqi3"
    [(set (match_operand:QI 0 "register_operand" "=r,r")
	  (minus:QI
	   (match_operand:QI 1 "register_operand" "0,0")
	   (match_operand:QI 2 "minirisc_add_operand" "r,i")))]
           ""
           "@
           SUB\\t%0 %2
           SUB\\t%0 %2"
	   [])

(define_insn "ashlqi3"
    [(set (match_operand:QI 0 "register_operand" "=r")
	  (ashift:QI
	   (match_operand:QI 1 "register_operand" "0")
	   (match_operand:QI 2 "immediate_operand" "i")))]
           ""
           "SL0\\t%0 %2")

(define_insn "ashrqi3"
    [(set (match_operand:QI 0 "register_operand" "=r")
	  (ashiftrt:QI
	   (match_operand:QI 1 "register_operand" "0")
	   (match_operand:QI 2 "immediate_operand" "i")))]
           ""
           "SR0\\t%0 %2")

(define_insn "rotlqi3"
    [(set (match_operand:QI 0 "register_operand" "=r")
	  (ashift:QI
	   (match_operand:QI 1 "register_operand" "0")
	   (match_operand:QI 2 "immediate_operand" "i")))]
           ""
           "ROL\\t%0 %2")

(define_insn "rotrqi3"
    [(set (match_operand:QI 0 "register_operand" "=r")
	  (ashiftrt:QI
	   (match_operand:QI 1 "register_operand" "0")
	   (match_operand:QI 2 "immediate_operand" "i")))]
           ""
           "ROR\\t%0 %2")


;; -------------------------------------------------------------------------
;; Logical operators
;; -------------------------------------------------------------------------

(define_insn "andqi3"
    [(set (match_operand:QI 0 "register_operand" "=r,r")
	  (and:QI (match_operand:QI 1 "register_operand" "0,0")
		  (match_operand:QI 2 "minirisc_add_operand" "r,i")))]
                  ""
                  "@
                  AND\\t%0 %2
                  AND\\t%0 %2"
            [])

(define_insn "xorqi3"
    [(set (match_operand:QI 0 "register_operand" "=r,r")
	  (xor:QI (match_operand:QI 1 "register_operand" "0,0")
		  (match_operand:QI 2 "minirisc_add_operand" "r,i")))]
                  ""
                  "@
                  XOR\\t%0 %2
                  XOR\\t%0 %2"
            [])

(define_insn "iorqi3"
    [(set (match_operand:QI 0 "register_operand" "=r,r")
	  (ior:QI (match_operand:QI 1 "register_operand" "0,0")
		  (match_operand:QI 2 "minirisc_add_operand" "r,i")))]
                  ""
                  "@
                  OR\\t%0 %2
                  OR\\t%0 %2"
            [])


;; -------------------------------------------------------------------------
;; Move instructions
;; -------------------------------------------------------------------------

;; We don't add SP as an operand that is going to be changed by the operation as we always call the
;; minirisc_decrease_sp() or the minirisc_decrease_sp() in the prologe/epiloge before the push/pop defined here.

(define_expand "movqi_push"
    [(set (mem:QI (reg:QI 1))
  	  (match_operand:QI 0 "register_operand" "r"))]
    ""
    {
      rtx insn;
      minirisc_push_emit (operands[0], insn);
      DONE;
    })

(define_expand "movqi_pop"
    [(set (match_operand:QI 1 "register_operand" "=r")
  	  (mem:QI (match_operand:QI 0 "register_operand" "r")))]
    ""
    {
      rtx insn;
      minirisc_pop_emit (operands[1], insn);
      DONE;
    })

(define_expand "movqi"
    [(set (match_operand:QI 0 "" "")
	  (match_operand:QI 1 "minirisc_general_movsrc_operand" ""))]
	  ""
	  "")

(define_insn "movqi_internal"
    [(set (match_operand:QI 0 "" "=r,r,r,r,r,W,A")
          (match_operand:QI 1 "minirisc_general_movsrc_operand" "r,i,Y,W,A,r,r"))]
          ""
          "@
          MOV\\t%0 %1
          MOV\\t%0 %1
          MOV\\t%0 %1
          LOAD\\t%0 %1
          LOAD\\t%0 %1
          STORE\\t%0 %1
          STORE\\t%0 %1"
          [])


;;(define_split
;;    [(set (match_operand:QI 0 "nonimmediate_operand" "")
;;	  (match_operand:QI 1 "minirisc_general_movsrc_operand" ""))]
;;	  "
;;          ((GET_CODE (operands[0]) == MEM) && (GET_CODE (XEXP (operands[0], 0)) == SYMBOL_REF)) ||
;;          ((GET_CODE (operands[1]) == MEM) && (GET_CODE (XEXP (operands[1], 0)) == SYMBOL_REF)) ||
;;          ((GET_CODE (operands[0]) == MEM) && (GET_CODE (XEXP (operands[0], 0)) == CONST)) ||
;;          ((GET_CODE (operands[1]) == MEM) && (GET_CODE (XEXP (operands[1], 0)) == CONST))
;;          "
;;	  [(const_int 0)]
;;	  {
;;	    minirisc_split_symbolic_move (operands[0], operands[1]);
;;	    DONE;
;;	  })


;; -------------------------------------------------------------------------
;; Call and Jump instructions
;; -------------------------------------------------------------------------

(define_expand "call"
    [(call (match_operand:QI 0 "memory_operand" "")
	   (match_operand 1 "general_operand" ""))]
           ""
           {
           gcc_assert (MEM_P (operands[0]));
           })

(define_insn "*call"
    [(call (mem:QI (match_operand:QI
		    0 "nonmemory_operand" "i,r"))
	   (match_operand 1 "" ""))]
           ""
           "@
           JSR\\t%0
           JSR\\t%0"
           [])

(define_insn "indirect_jump"
    [(set (pc) (match_operand:QI 0 "nonimmediate_operand" "r"))]
    ""
    "JMP\\t%0")

(define_insn "jump"
    [(set (pc) (label_ref (match_operand 0 "" "")))]
    ""
    "JMP\\t%l0")

;; -------------------------------------------------------------------------
;; Compare instructions
;; -------------------------------------------------------------------------

(define_constants
  [(CC_REG 11)])

(define_expand "cbranchqi4"
  [(set (reg:CC CC_REG)
        (compare:CC
         (match_operand:QI 1 "general_operand" "")
         (match_operand:QI 2 "general_operand" "")))
   (set (pc)
        (if_then_else (match_operator 0 "comparison_operator"
                       [(reg:CC CC_REG) (const_int 0)])
                      (label_ref (match_operand 3 "" ""))
                      (pc)))]
  ""
  "
  /* Force the compare operands into registers.  */
  if (GET_CODE (operands[1]) != REG)
	operands[1] = force_reg (QImode, operands[1]);
  if (GET_CODE (operands[2]) != REG)
	operands[2] = force_reg (QImode, operands[2]);
  ")

(define_insn "*cmpqi"
  [(set (reg:CC CC_REG)
	(compare
	 (match_operand:QI 0 "register_operand" "r")
	 (match_operand:QI 1 "register_operand"	"r")))]
  ""
  "CMP\\t%0 %1")

;; -------------------------------------------------------------------------
;; Branch instructions
;; -------------------------------------------------------------------------

(define_code_iterator cond [ne eq lt ltu gt gtu ge le geu leu])
(define_code_attr CC [(ne "JNZ") (eq "JZ") (lt "JL") (ltu "JC")
		      (gt "JH") (gtu "JG") (ge "JHE") (le "JLE")
		      (geu "JNC") (leu "JSE") ])

(define_insn "*<cond:code>"
  [(set (pc)
	(if_then_else (cond (reg:CC CC_REG)
			    (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
{
  return "<CC>\\t%l0";
})


;; -------------------------------------------------------------------------
;; Prologue & Epilogue
;; -------------------------------------------------------------------------

(define_expand "prologue"
    [(clobber (const_int 0))]
    ""
    "
{
  minirisc_expand_prologue ();
  DONE;
}
")

(define_expand "epilogue"
    [(return)]
    ""
    "
{
  minirisc_expand_epilogue ();
  DONE;
}
")

(define_insn "ret"
    [(return)]
    "reload_completed"
    "RTS")

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
    "RTI")

(define_insn "sti"
    [(unspec_volatile [(const_int 0)] UNSPECV_STI)]
    ""
    "STI")

(define_insn "cli"
    [(unspec_volatile [(const_int 0)] UNSPECV_CLI)]
    ""
    "CLI")

;; Local Variables:
;; mode: lisp
;; indent-tabs-mode: t
;; End:
