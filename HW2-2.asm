# Where Harry Met Sally
#
#
# This program determines when Harry and Sally first resided in the same city
# and reports the ID of that city.
#
# Required output register usage:
# $2: common city ID
#
# 18SEP17					Isaac Webb

.data
Harry:  .alloc  10               # Allocate static space for 5 moves
Sally:  .alloc  10               # Allocate static space for 5 moves

.text
# Register Definitions:
# $1:     Harry timeline address
# $2:     Final answer
# $3:     Correct answer
# $4:     Sally timeline address
# $5-$8:  Temporary calculations
# $11:    Harry
# $12:    Sally
# $13:    WhereMet caller's address
# $14:    AlignTimeline caller's address
WhereMet:	       addi $1, $0, Harry     # Initialize Harry's counter
                 addi $11, $0, Harry    # Save Harry's start address for later calculations
                 addi $4, $0, Sally     # Initialize Sally's counter
                 addi $12, $0, Sally    # Save Sally's start address for later calcultaions
                 addi $2, $0, -99       # Give the city a default value
        	       swi  586               # Create timelines and store them
                 add  $13, $0, $31      # Save the caller's address
                 jal  AlignTimelines    # Align the timelines
MainLoop:        sub  $5, $1, $11       # Get Harry's offset
                 slti $5, $5, 40        # Exit loop if we are at the end of Harry's array
                 beq  $5, $0, Submit
                 sub  $5, $4, $12       # Get Sally's offset
                 slti $5, $5, 40        # Exit loop if we are at the end of Sally's array
                 beq  $5, $0, Submit
                 lw   $5, 4($1)         # Get Harry's current city
                 lw   $6, 4($4)         # Get Sally's current city
                 bne  $5, $6, Increment # If they aren't equal, continue the loop
                 add  $2, $0, $5        # Save the answer
                 j    Submit
Increment:       jal  AdvanceTimeline   # Advance the timelines and align them
                 jal  AlignTimelines
                 j    MainLoop
Submit:          swi  594		            # Give answer
                 add  $31, $0, $13      # Recall the caller's address
                 jr   $31               # Return to the caller
AdvanceTimeline: sub  $5, $1, $11       # Check how far in Harry's array we are
                 slti $5, $5, 32
                 bne  $5, $0, HarryElse # We haven't reached the array end yet
                 addi $4, $4, 8         # Increment Sally's counter
                 jr   $31               # Return to the caller
HarryElse:       sub  $5, $4, $12       # Check how far in Sally's array we are
                 slti $5, $5, 32
                 bne  $5, $0, SallyElse # We haven't reached the array end yet
                 addi $1, $1, 8         # Increment Harry's counter
                 jr   $31               # Return to the caller
SallyElse:       lw   $5, 8($1)         # Load Harry's next move
                 lw   $6, 8($4)         # Load Sally's next move
                 beq  $5, $6, Equal     # If they're equal, increment both counters
                 slt  $5, $5, $6        # Compare who moves first
                 beq  $5, $0, Else      # See who moves first
                 addi $1, $1, 8         # Increment Harry's counter
                 jr   $31               # Return to the caller
Equal:           addi $1, $1, 8         # Increment Harry's counter
Else:            addi $4, $4, 8         # Increment Sally's counter
                 jr   $31               # Return to the caller
AlignTimelines:  add  $14, $0, $31      # Save the caller's address
AlignLoop:       sub  $5, $1, $11       # Get Harry's offset
                 slti $5, $5, 32        # If Harry's offset is greater than 32, exit
                 beq  $5, $0, AlignExit
                 sub  $6, $4, $12       # Get Sally's offset
                 slti $6, $6, 32        # If Sally's offset is greater than 32, exit
                 beq  $6, $0, AlignExit
Current:         lw   $5, 8($1)         # See if Harry isn't current
                 lw   $6, 0($4)
                 slt  $5, $6, $5
                 beq  $5, $0, Advance
                 lw   $5, 0($1)         # See if Sally isn't current
                 lw   $6, 8($4)
                 slt  $5, $5, $6
                 bne  $5, $0, AlignExit
Advance:         jal  AdvanceTimeline   # Call advanceTimeline
                 j    AlignLoop
AlignExit:       add  $31, $0, $14      # Recall the caller's address
                 jr   $31
