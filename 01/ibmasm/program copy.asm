         YREGS                                                                  
PROGRAM  CSECT                                                                  
         STM   R14,R12,12(R13)         
         LR    R12,R15                 
         USING PROGRAM,R12                                                     
         ST    R13,SAVE+4              
         LR    R14,R13                                                          
         LA    R13,SAVE                        
         ST    R13,8(,R14)                  
         OPEN  (SYSOUT,(OUTPUT),SYSIN,(INPUT))       
         J     MAIN
* --------------------------------------------------------------------
END      EQU   *
         CLOSE (SYSOUT,,SYSIN)
         L     R13,SAVE+4                               
         LM    R14,R12,12(R13)                             
         LA    R15,0                                  
         BR    R14                                                              
* --------------------------------------------------------------------  
* ------- MAIN START -------
*
MAIN     EQU   *
         ZAP   SUM,=P'0'

ROW      EQU   *   
* RESETTING BLACK MAGIC
*         MVC   NAHI+2,=H'1'
         LA    R3,ROWNUM
*         MVC   BNEXTD+2,NEXTD-BNEXTD

         MVC   ROWNUM,=C'00'
         GET   SYSIN
*         MVC   WTOTEXT(15),0(R1)
*         BAL   R14,PRINT
         LH    R2,0(R1)
         AHI   R1,4
         AHI   R2,-4
         AR    R2,R1     R2 END OF STR
         LR    R6,R2     SAVE END OF STR IN R6
         LR    R5,R1     SAVE START OF STR IN R5

LOOP     EQU   *
         CLI   0(R1),C'0'
         BL    NEXTC
         CLI   0(R1),C'9'
         BH    NEXTC
* IF YOU ARE HERE YOU FOUND A DIGIT
         MVC   0(1,R3),0(R1)
*BLASTD   B     LASTD

         LR    R1,R6
         AHI   R1,-1
LOOP2    EQU   *
         CLI   0(R1),C'0'
         BL    NEXTC2
         CLI   0(R1),C'9'
         BH    NEXTC2
* IF YOU ARE HERE YOU FOUND A DIGIT
         MVC   1(1,R3),0(R1)
         B     ROWDONE



* NEXT CHARACTER
NEXTC    EQU   *
         AHI   R1,1
         J     LOOP

* NEXT CHARACTER
NEXTC2    EQU   *
         AHI   R1,-1
         J     LOOP2





* NUM FOUND, SUM
ROWDONE  EQU   *
         PACK  APL2,ROWNUM
         AP    SUM,APL2
         MVC   WTOTEXT(2),ROWNUM
         BAL   R14,PRINT
         J     ROW

EOFSYSIN EQU   *
         UNPK  ACL15,SUM
         MVC   WTOTEXT(15),ACL15
         OI    WTOTEXT+14,X'F0'
         BAL   R14,PRINT
         J     END
* ------- MAIN END -------

* --------------------------------------------------------------------  

* ------- SUB ROUTINES START -------
* STACKNUM IN R5           TODO QUEUE
* STACKITEM IN R6
*
PRINT    EQU   *
         ST    R1,R1SAVE
         WTO   MF=(E,WTOBLOCK)
         PUT   SYSOUT,WTOTEXT
         MVI   WTOTEXT,C' '
         MVC   WTOTEXT+1(L'WTOTEXT-1),WTOTEXT
         L     R1,R1SAVE
         BR    R14
*
* ------- SUB ROUTINES END ------- 

* --------------------------------------------------------------------   
SYSIN    DCB   DDNAME=SYSIN,DSORG=PS,MACRF=GL,RECFM=VT,LRECL=80,       +
               EODAD=EOFSYSIN
SYSOUT   DCB   DDNAME=SYSOUT,DSORG=PS,MACRF=PM,RECFM=FT,LRECL=80                
*
         LTORG
*       
SAVE     DC    18F'0'                 
*
R1SAVE   DS    F
SUM      DS    PL8
APL2     DS    PL2
ACL15    DS    CL15
ROWNUM   DS    CL2
*
WTOBLOCK EQU   *
         DC    H'84'
         DC    H'0'
WTOTEXT  DC    CL80' '
*
         END                                                                    
