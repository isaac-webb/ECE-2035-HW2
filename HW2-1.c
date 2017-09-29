/*    When Harry Met Sally

This program determines when Harry and Sally first resided in the same city
and prints the ID of that city.

18SEP17	               Isaac Webb   */

#include <stdio.h>
#include <stdlib.h>

// Declare custom functions
void alignTimelines(int timelineOne[], int *indexOne, int timelineTwo[], int *indexTwo);
void advanceTimeline(int timelineOne[], int *indexOne, int timelineTwo[], int *indexTwo);

/* City IDs used in timelines. */
enum Cities{ London, Boston, Paris, Atlanta, Miami,
             Tokyo, Metz, Seoul, Toronto, Austin };

int main(int argc, char *argv[]) {
   int HarryTimeline[10];
   int SallyTimeline[10];
   int NumNums;
   int Load_Mem(char *, int *, int *);
   int City = -99;
   int harryIndex = 0, sallyIndex = 0;

   // Print error messages if there aren't enough arguments or
   // text file is formatted incorrectly
   if (argc != 2) {
     printf("usage: ./HW2-1 valuefile\n");
     exit(1);
   }
   NumNums = Load_Mem(argv[1], HarryTimeline, SallyTimeline);
   if (NumNums != 20) {
     printf("valuefiles must contain 20 entries\n");
     exit(1);
   }

   // Make sure the timelines overlap initially
   alignTimelines(HarryTimeline, &harryIndex, SallyTimeline, &sallyIndex);

   // Loop through all of the moves
   while (harryIndex < 10 && sallyIndex < 10) {
     // If the cities match, set the result and exit the loop
     if (HarryTimeline[harryIndex + 1] == SallyTimeline[sallyIndex + 1]) {
       City = HarryTimeline[harryIndex + 1];
       break;
     }

     // Force the advancement of one of the timelines and then make sure they
     // overlap
     advanceTimeline(HarryTimeline, &harryIndex, SallyTimeline, &sallyIndex);
     alignTimelines(HarryTimeline, &harryIndex, SallyTimeline, &sallyIndex);
   }

   printf("City in common the earliest: %d\n", City);
   exit(0);
}

/*
  Advance the timeline indicies based on whichever person moves sooner
 */
void advanceTimeline(int timelineOne[], int *indexOne, int timelineTwo[], int *indexTwo) {
  // When the bounds are reached, always advance the other index
  if (*indexOne >= 8) {
    *indexTwo += 2;
    return;
  }
  if (*indexTwo >= 8) {
    *indexOne += 2;
    return;
  }

  // See who moves first and advance their timeline
  if (timelineOne[*indexOne + 2] < timelineTwo[*indexTwo + 2]) {
    *indexOne += 2; // Increment first counter
  } else if (timelineTwo[*indexTwo + 2] < timelineOne[*indexOne + 2]){
    *indexTwo += 2; // Increment second counter
  } else {
    // Increment both countera
    *indexOne += 2;
    *indexTwo += 2;
  }
}

/*
  Corrects timelines and makes sure they overlap so comparisons can occur
 */
 void alignTimelines(int timelineOne[], int *indexOne, int timelineTwo[], int *indexTwo) {
   // See who moves first and make sure timelines overlap
   while (*indexOne < 8 && *indexTwo < 8 && (timelineOne[*indexOne + 2] <= timelineTwo[*indexTwo]
          || timelineTwo[*indexTwo + 2] <= timelineOne[*indexOne])) {
     advanceTimeline(timelineOne, indexOne, timelineTwo, indexTwo);
   }
 }

/* This routine loads in up to 20 newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray1[], int IntArray2[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
     printf("%s could not be opened; check the filename\n", InputFileName);
     return 0;
   } else {
     for (N=0; N < 20; N++) {
       NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
       if (NumVals == 2)
	 if (N < 10)
	   IntArray1[N] = Value;
	 else
	   IntArray2[N-10] = Value;
       else
	 break;
     }
     fclose(FP);
     return N;
   }
}
