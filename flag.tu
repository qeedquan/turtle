// Turtle Graphics Sample Application
// United States Flag

// This function draws each star
Function DrawStar
{
// Draw five points to make each star
   Loop 5
   {
         Rotate 54

         Draw 5

         Rotate -126

         Draw 5

   }

}     

 

// This function draws a row of stars
Function DrawRowOfStars

{

   // Draw ten columns of stars

   Loop 10

   {

   Call DrawStar

   Move 20, -1

   }

 

}

 

// This function draws 5 rows of stars
Function DrawRowsOfStars
{
   // Draw the 50 stars of the United States
   MoveTo 27,30

   Rotate 90

   Color White

   // Draw five rows of stars
   Loop 5

   {

      Call DrawRowOfStars

      Move -210,20

   }

}

 

// This function draws a line on the flag
Function DrawLine
{

   Loop 14
   {

      Draw 410

      Move -410,1

   }

}

 

// This function draws 13 lines
Function DrawLines

{

   // Draw the 13 lines of the US flag alternating red and white

   MoveTo 20,25

   Rotate -90

 

   Loop 6
   {
      // Lines are 14 pixels high
      Color Red
      Call DrawLine

 

      Color White
      Call DrawLine

   }

 

   // Draw the final red line
   Color Red
   Call DrawLine

}

 

// This function draws the blue background
Function DrawBlueBackground
{
   // Draw the blue background for under the stars
   Color Blue
   MoveTo 20,25
   Loop 97
   {
      Draw 210
      Move -210,1
   }

}

// Draw The American Flag by Calling The Appropriate Functions
Call DrawLines
Call DrawBlueBackground
Call DrawRowsOfStars

