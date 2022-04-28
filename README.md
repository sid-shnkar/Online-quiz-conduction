# Online-quiz-conduction
A project made using C programming language to mimic the online quiz conduction

# Description

# Steps to run the program

1. Compile and run the program in your IDE.
2. Login as admin by entering the following credentials:
             Username:  admin
             Password:  admin
3. Choose option  ‘3’  to add other user credentials. Add as many as you want.
4. Choose option  ‘1’  to add questions to question bank 1 or 2 , whichever you want.
   One way to enter a question is as follows:
      a. Enter question statements line by line , choosing ‘Y’ everytime to add another line.
      b. Then choose ‘Y’ again and type in “Options:” without quotes
      c. Type in options line by line , as depicted below (without * and / )
                        (A)  /*Enter option here */          ( in 1st line)
                        (B)  /*Enter option here */          ( in 2nd line)
                           and so on...till option D
      d. Now choose ‘Y’ again and type in the word “EOQ” without double quotes to
         indicate the end of each question (everything till here will be stored in the the text file).
      e.  Enter the correct option and marks allotted to that question and specify the type of that question(M/S/N/Q).
      f.  Repeat the above steps to add another question .
5. Choose option ‘4’ to create a feedback form which will be used for receiving feedback from the user.
6. Exit the program by choosing option ‘7’. Then login as another user to attempt the quiz. Follow the instructions as displayed in the program.

# Name and location of files used in the program

* User details will be stored in a .dat file named  “ User_details.DAT “ 
* Question bank 1 will be stored in the form of two files:  “Que_bank_1.DAT“  and  “ Que_bank_1.txt “  .
* Question bank 2 will be stored in the form of two files:  “Que_bank_2.DAT “  and  “Que_bank_2.txt“ .
* Report card will be stored in a text file named  “username.txt“ where the username will be different for each user.
* Time limit for question bank 1 will be stored in a text file named “Time_limit_qb1.txt” while for question bank 2, file name will be  “Time_limit_qb2.txt”.
* Feedback questions will be stored in a text file named “Feedback_form.txt” while feedback responses received from the user will be stored in file named      “Feedback_responses.DAT”
* User data required for viewing the quiz analytics will be stored in a file named “Quiz_analytics.DAT” .

# Program execution pics

![c5](https://user-images.githubusercontent.com/104520126/165797162-ed605f08-8326-4de4-8160-d9aa92c94f9c.png)
![c6](https://user-images.githubusercontent.com/104520126/165797193-05b3e936-2526-4499-a7a6-0aead899fd25.png)
![c8](https://user-images.githubusercontent.com/104520126/165797224-745dc69e-b1cf-44a6-8278-a610c7efa8af.png)
![cf](https://user-images.githubusercontent.com/104520126/165797245-b05a4141-d0d4-4001-b481-a155d87c928d.png)







