;; This file contains target machine instruction templates. We are forced to provide at least one condition/instruction
;; otherwise the build will fail (zero-sized array error)

;; We will extend this file later, for now we only provide two
;; instructions, a simple add (reg = reg + reg) that uses a dummy
;; condition and a nop (interestingly, gc refuses to build without it)
;; -----------------------------------------------------------------------

(include "constraints.md")

;; -----------------------------------------------------------------------
;; Attributes
;; 
;; Define the type of instructions we are dealing with. For the time
;; being we distinguish only the most basic instructions without paying
;; attention to fancy extras.
;; -----------------------------------------------------------------------

(define_attr "type"
  "unknown,nop,mov,arith,logic,sex,test,mult,load,store,cbranch,ubranch,
  call, libcall"
  (const_string "unknown"))

(define_attr "mode"
  "unknown,none,SI"
  (const_string "unknown"))

(define_attr "length"
  "" (const_int 1))

;; -----------------------------------------------------------------------
;; Moves/loads/stores
;; 
;; Alright, movsi is like a godmother of all transfer patterns.
;; Depending on the operands it captures simple reg-reg moves as well as loads
;; and stores (and more).
;; -----------------------------------------------------------------------

(define_insn "movst_insn"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=r,r,r,m,m")
        (match_operand:SI 1 "general_operand" "i,r,m,r,J"))]
  "minirisc_valid_movsi_insn(SImode, operands)"
  "@
  %0 = %1;
  %0 = %1;
  %0 = port32[%1];
  port32[%0] = %1;
  port32[%0] = %1;"

  [(set_attr "type" "mov,mov,load,store,store")
   (set_attr "length" "1,1,8,8,8")]
)

;; expand properly depending on the operand types
(define_expand "movsi"
  [(set (match_operand:SI 0 "general_operand" "")
        (match_operand:SI 1 "general_operand" ""))]
  ""
  "{
      minirisc_expand_movsi(operands);
      DONE;
  }"
)

;; -----------------------------------------------------------------------
;; Unconditional branches
;; 
;; NOTE: the most important instructions to provide at the begining
;; is a direct unconditional jump.
;; -----------------------------------------------------------------------

;; direct unconditional
(define_insn "jump"
  [(set (pc) (label_ref (match_operand 0)))]
  ""
  "jump (%0);"
  [(set_attr "type" "ubranch")]
)

;; indirect unconditional
(define_insn "indirect_jump"
  [(set (pc) (match_operand:SI 0 "register_operand" "r"))]
  ""
  "jump (%0);"
  [(set_attr "type" "ubranch")]
)

;; later for call instructions
(define_insn "return"
  [(return)]
  ""
  "jump (r63);"
  [(set_attr "type" "ubranch")]
)


;; -----------------------------------------------------------------------
(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (plus:SI (match_operand:SI 1 "register_operand" "r")
                 (match_operand:SI 2 "register_operand" "r")))]
  "minirisc_dummy_insn_cond(SImode)"
  "%0 = %1 + %2;"
)
;; -----------------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
)