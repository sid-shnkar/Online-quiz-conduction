#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>
struct usercredentials
{
    char username[100], password[100];
};
struct usercredentials uc;
struct quebank
{
    char correct_option,question_type,subjective_answer[100],msq_options[6];
    float marks,numerical_answer,partial_marks,negative_marks;
    int msq_correctoptsnumber;
};
struct quebank q;
struct reportcard
{     
    int attempted_questions, correct_answers, wrong_answers,total_questions,partial_correct_answers;
     float total_score;
};
struct reportcard rc;
struct feedback
{
    int fboptions[5];
    char usercomment[100];
};
struct feedback fb;
struct quizanalytics
{
    float tot_score;
    char user_name[100];     
};
struct quizanalytics qa;
// Following are the function prototypes
void login_details();
void admin_rights();
void attempt_quiz(struct reportcard rcard,char rcusrname[100],int *flag);
void report_card(char rcusrname[100],struct reportcard rcattempt_quiz,float,int *storetime);
void calculate_time(int *storetime,int number);
void collect_feedback();
void create_feedback();
void view_feedback();
void quiz_analytics();
void analytics_system(char asusrname[100],float,int);
int check_time(time_t passedtime,int *timeupornot,int tmlimit);

/*
 * Function: main()
 * ---------------------------------------------------------------------------------------
 * Here time_t is a typedef defined in the <time.h> header file.                 
*/

int main()
{
     time_t t;             // to store system time
    time(&t);
    printf("\n\t\t*****************************************");
    printf("\n\t\t\t      WELCOME ");
     printf("\n\t\t\t        TO ");
     printf("\n\t\t\t  ONLINE QUIZ PORTAL  ");
     printf("\n\t\t*****************************************");
     printf("\n\t\t___________________________________________");
     printf("\n\t\tCurrent Date and Time: %s ", ctime(&t)) ;
     printf("\n\t\t       Time to become a Champion !!     ") ;
     printf("\n\t\t___________________________________________");
     printf("\nEnter your login details:\n");
    login_details();
    return 0;
}

/*
 * Function: login_details()
 * ---------------------------------------------------------------------------------------
 * Here 'usrname' and 'passwrd'are accepted from the user.As usual no space is allowed.
 * variable 'flag' will keep check of whether the user has chosen to reattempt the quiz or not.
 * validuser=1 impliesthat user has entered valid username and password.
 * checkadmin=1 implies that user running the program is the admin.
 * Here User_details.DAT file contains the username and password of each user.   
 * attempt_quiz() function is passed a structure variable 'rc' of structure reportcard which
   will contain information like attempted questions, total score etc.
 * Also 'usrname' of the user currently logged in along with 'flag' is passed to attempt_quiz() .           
*/

void login_details()
{
    char usrname[100], passwrd[100],reattempt_choice;
    int  validuser = 0, checkadmin = 0,flag=0;
    FILE *login_file;
    login_file = fopen("User_details.DAT", "rb+");
    if (login_file == NULL)
    {
        login_file = fopen("User_details.DAT", "wb+");
        if (login_file == NULL)
        {
            printf("Oops! cannot open file...please try again\n");
            exit(1);
        }
    }
    fseek(login_file, 0, SEEK_END);
    printf("Enter username \n");
    scanf("%s",usrname);            
    printf("Enter password \n");
    scanf("%s",passwrd); 
    if (((strcmp(usrname, "admin")) == 0 && (strcmp(passwrd, "admin") == 0)))
    {
         checkadmin=1;
         validuser=1;
    }        
    fseek(login_file,0,SEEK_SET);
    while (fread(&uc, sizeof(uc), 1, login_file) == 1)
    {
        if (((strcmp(uc.username, usrname)) == 0 && (strcmp(uc.password, passwrd) == 0)))
        {
            validuser = 1;
            break;
        }
    }
    printf("\nVerifying your login details....Please wait....\n");
    fclose(login_file);
    if (validuser == 1 && checkadmin == 0)
    {
        printf("You are a normal user\n\n");
        attempt_quiz(rc,usrname,&flag);
        printf("Do you want to reattempt the quiz? (only 1 reattempt allowed)(Y/N) \n");
        scanf(" %c",&reattempt_choice);
        if(reattempt_choice=='Y')
        {   
            flag++;
            attempt_quiz(rc,usrname,&flag);
            collect_feedback();
        }
        else if(reattempt_choice=='N')
        {   
            collect_feedback();
        }
        printf("\nYou have successfully log out\n");
        printf("_____________________________________________________________________\n\n");
    }
    else if (validuser == 1 && checkadmin == 1)
    {
        printf("You are the admin\n\n");
         admin_rights();
    }
    else{
        printf("Wrong username or password entered !\n\n");
        }
}

/*
 * Function: admin_rights()
 * -------------------------------------------------------------------------------------
 * line[100] temporarily stores each line of question statement.
 *  The word 'EOQ' typed explicitly will denote end of question for each question.                   
*/

void admin_rights()
{   
    char another, choice,line[100],option_choice,time_limit[20]; 
    int que_bank_number,i=0;
    FILE *que_bank_1, *que_bank_2, *user_credentials,*que_bank_txt,*time_limit_txt;
    printf("Welcome admin!  \n");
    que_bank_1 = fopen("Que_bank_1.DAT", "rb+");
    if (que_bank_1 == NULL)
    {
        que_bank_1 = fopen("Que_bank_1.DAT", "wb+");
        if (que_bank_1 == NULL)
        {
            printf("Oops! cannot open file...please try again\n");
            exit(1);
        }
    }
    que_bank_2 = fopen("Que_bank_2.DAT", "rb+");
    if (que_bank_2 == NULL)
    {
        que_bank_2 = fopen("Que_bank_2.DAT", "wb+");
        if (que_bank_2 == NULL)
        {
            printf("Oops! cannot open file...please try again\n");
            exit(1);
        }
    }
    user_credentials = fopen("User_details.DAT", "rb+");
    if (user_credentials == NULL)
    {
        user_credentials = fopen("User_details.DAT", "wb+");
        if (user_credentials == NULL)
        {
            printf("Oops! cannot open file...please try again\n");
            exit(1);
        }
    }
    
    while (1)
    {
        printf("********************************************************************\n");
        printf("List of privileges available to you:\n");
        printf("1.Add a new question\n");
        printf("2.List all the questions\n");
        printf("3.Add a new user's credentials\n");
        printf("4.Create a feedback form\n");
        printf("5.View user's feedback \n");
        printf("6.View Quiz analytics\n");
        printf("7. Exit\n");
        printf("********************************************************************\n");
        printf("Your choice\n");
        scanf(" %c",&choice);
        switch (choice)
        {
        case '1':

            printf("\nIn which bank do you want to add questions(1.First 2.Second)?\n");
            scanf("%d",&que_bank_number);
            if (que_bank_number == 1)
            {
                fseek(que_bank_1, 0, SEEK_END);
               que_bank_txt=fopen("Que_bank_1.txt","a+");
               time_limit_txt=fopen("Time_limit_qb1.txt","w");
            }    
            else
              {  
                  fseek(que_bank_2, 0, SEEK_END);
                  que_bank_txt=fopen("Que_bank_2.txt","a+");
                  time_limit_txt=fopen("Time_limit_qb2.txt","w");
              }   
            another = 'Y';
            printf("\nImportant note:\n");
            printf("1.Enter question statement line by line,then the options(for MCQ and MSQ).\n");
            printf("2.After typing options, type the word EOQ in next line to denote end of question.\n");
            printf("Now proceed further....\n\n");
            while (another == 'Y')
            {
                while(another == 'Y')
                {   
                    printf("\nEnter a line:\n");
                    fflush(stdin);
                    fgets(line,99,stdin);
                    fprintf(que_bank_txt,"%s",line);
                    printf("\nAdd another line ?(Y/N)");
                    scanf(" %c",&another);
                }
                printf("\nEnter marks allotted to this question\n");
                scanf("%f", &q.marks);
                printf("Is this a MCQ/subjective/numerical type/MSQ question?(M/S/N/Q)\n");
                scanf(" %c",&choice);
                if(choice=='M')
               {   
                   q.question_type='M';
                   printf("Correct option for this question?\n");
                   scanf(" %c", &q.correct_option);
               }
               else if(choice=='S')
               {  
                  q.question_type='S';
                  printf("Correct answer for this question?\n");
                  fflush(stdin);
                  fgets(q.subjective_answer,99,stdin);
               } 
               else if(choice=='N')
               {    
                   q.question_type='N';
                   printf("Correct answer for this question?\n");
                    scanf("%f",&q.numerical_answer);
               }
               else if(choice=='Q')
               {   
                   printf("\nNo. of correct option(s) for this question\n");
                   scanf("%d",&q.msq_correctoptsnumber);
                   printf("\nCorrect option(s) in one word for this question?\n");
                   q.question_type='Q';
                    scanf("%s",q.msq_options);
                    printf("\nThank you for giving the options\n");
                    printf("\nPartial marks alloted to this question?\n");
                    scanf("%f",&q.partial_marks);
                    printf("\nNegative marks alloted to this question?\n");
                    scanf("%f",&q.negative_marks);
               }
                if (que_bank_number == 1)
                    fwrite(&q, sizeof(q), 1, que_bank_1);
                else
                    fwrite(&q, sizeof(q), 1, que_bank_2);
                printf("\nAdd another question (Y/N)");
               scanf(" %c",&another);
            }
            printf("\nTime limit for this question bank?(total in seconds)\nExample: 20s\n");
            scanf("%s",time_limit);
            fprintf(time_limit_txt,"%s",time_limit);
            fclose(time_limit_txt);
            fclose(que_bank_txt);
            printf("\nQuestions added successfully..\n");
            break;
        case '2':
            printf("\nWhich bank do you want to display(1.First 2.Second) ?\n");
            scanf("%d", &que_bank_number);
            if (que_bank_number == 1)
            {
                fseek(que_bank_1,0,SEEK_SET);
                que_bank_txt=fopen("Que_bank_1.txt","a+");
                time_limit_txt=fopen("Time_limit_qb1.txt","r");
                while (fread(&q, sizeof(q), 1, que_bank_1) == 1)
                {   
                    printf("**********************************************************************************\n");
                    while(fgets(line,99,que_bank_txt)!=NULL)
                    {
                         if(strcmp(line,"EOQ\n")==0)
                          break;
                          printf("%s",line);
                    }
                    if(q.question_type=='M')
                    printf("\nCorrect option: %c\nMarks alloted to this question: %f\n",q.correct_option,q.marks);
                    else if(q.question_type=='S')
                    printf("\nCorrect answer: %s\nMarks alloted to this question: %f\n",q.subjective_answer,q.marks);
                    else if(q.question_type=='N')
                    printf("\nCorrect answer: %f\nMarks alloted to this question: %f\n",q.numerical_answer,q.marks);
                    else if(q.question_type=='Q')
                    {
                        printf("\nCorrect option(s) are:\n");
                        for(int i=0;i<q.msq_correctoptsnumber;i++)
                        printf("%c ",q.msq_options[i]);
                        printf("\nMaximum marks alloted to this question: %f\n",q.marks);
                        printf("\nPartial marks alloted to this question: %f\n",q.partial_marks);
                        printf("\nNegative marks alloted to this question: %f\n",q.negative_marks);
 
                    }
                    printf("*********************************************************************************\n\n");
                }
                printf("\nTime limit for this question bank(total in seconds):\n");
                fgets(line,99,time_limit_txt);
                printf("%s",line);
            }
            else
            {
                rewind(que_bank_2);
                que_bank_txt=fopen("Que_bank_2.txt","a+");
                time_limit_txt=fopen("Time_limit_qb2.txt","r");
                while (fread(&q, sizeof(q), 1, que_bank_2) == 1)
                {   
                    printf("**********************************************************************************\n");
                    while(fgets(line,99,que_bank_txt)!=NULL)
                    {                        
                         if(strcmp(line,"EOQ\n")==0)
                          break;
                          printf("%s",line);
                    }
                    if(q.question_type=='M')
                    printf("\nCorrect option: %c\nMarks alloted to this question: %f\n",q.correct_option,q.marks);
                    else if(q.question_type=='S')
                    printf("\nCorrect answer: %s\nMarks alloted to this question: %f\n",q.subjective_answer,q.marks);
                    else if(q.question_type=='N')
                    printf("\nCorrect answer: %f\nMarks alloted to this question: %f\n",q.numerical_answer,q.marks);
                    else if(q.question_type=='Q')
                    {
                        printf("\nCorrect option(s) are:\n");
                        for(int i=0;i<q.msq_correctoptsnumber;i++)
                        printf("%c ",q.msq_options[i]);
                        printf("\nMaximum marks alloted to this question: %f\n",q.marks);
                        printf("\nPartial marks alloted to this question: %f\n",q.partial_marks);
                        printf("\nNegative marks alloted to this question: %f\n",q.negative_marks);
                    }
                    printf("*********************************************************************************\n\n");
                }
                printf("\nTime limit for this question bank(in seconds):\n");
                fgets(line,99,time_limit_txt);
                printf("%s",line);
            }
            fclose(que_bank_txt);
            fclose(time_limit_txt);
            printf("\n\nAll Questions of selected question bank have been displayed...\n\n");
            break;
        case '3': 
            fseek(user_credentials, 0, SEEK_END);
            another = 'Y';
            while (another == 'Y')
            {
                printf("\nEnter username \n");
                scanf("%s",uc.username);
                printf("Enter password \n");
                scanf("%s",uc.password);
                fwrite(&uc, sizeof(uc), 1, user_credentials);
                printf("\nAdd another user details (Y/N)");
                scanf(" %c",&another);
            }

            printf("\nThank you for giving user details admin!\n");
            break;
        case '4':
                  create_feedback();
                  break;    
        case '5':
                   view_feedback();
                   break;
        case '6': 
                  quiz_analytics(); 
                  break;              
        case '7':
            fclose(que_bank_1);
            fclose(que_bank_2);
            fclose(user_credentials);
            printf("\nYou have successfully log out\n");
            printf("___________________________________________________________________\n\n");
            exit(0);
        }
    }
}

/*
 * Function: attempt_quiz()
 * ---------------------------------------------------------------------------------------
 * flag=0 implies that it is the first attempt by the user while flag=1 implies that it is a
   re-attempt by the user.                
*/

void attempt_quiz(struct reportcard rcard,char rcusrname[100],int *flag)
{
    printf("\nGet ready to attempt the quiz!\n");
    char attempt_choice,line[100],analytics_choice,mcq_response,subjective_response[100],msq_response,msq_choice='Y',temp[20];
    float max_score=0.0,numerical_response;
    int store_time[2],check,counter,timeup=0,timelimit,position;
    time_t start_time,end_time,current_time;
   rcard.attempted_questions=0, rcard.correct_answers=0, rcard.wrong_answers=0,rcard.total_score=0.0,rcard.total_questions=0,rcard.partial_correct_answers=0;
   FILE *question_bank,*que_bank_txt,*time_limit_txt;
   time(&start_time);
   if(*flag==0)
  { 
   question_bank = fopen("Que_bank_1.DAT", "rb+");
   que_bank_txt=fopen("Que_bank_1.txt","a+");
   time_limit_txt=fopen("Time_limit_qb1.txt","r");
    if (question_bank == NULL)
    {
        question_bank = fopen("Que_bank_1.DAT", "wb+");
        if (question_bank == NULL)
        {
            printf("Oops! cannot open file...please try again\n");
            exit(1);
        }
    }
  } 
  else if(*flag==1)
  { 
    question_bank = fopen("Que_bank_2.DAT", "rb+");
    que_bank_txt=fopen("Que_bank_2.txt","a+");
    time_limit_txt=fopen("Time_limit_qb2.txt","r");
    if (question_bank == NULL)
    {
        question_bank = fopen("Que_bank_2.DAT", "wb+");
        if (question_bank == NULL)
        {
            printf("Oops! cannot open file...please try again\n");
            exit(1);
        }
    }
  }         
            position=0;
            fgets(line,99,time_limit_txt);
            for(int i=0; line[i] != 's'; i++)
            {
               temp[position++] = line[i];
            }
             temp[position] = '\0';
            timelimit = atoi(temp);
            calculate_time(store_time,timelimit);
            printf("-----------------------------------------------------------------------------------\n");
            printf("Time limit for quiz: %d minutes %d seconds\n",store_time[0],store_time[1]);
            printf("Note: For MSQ , partial marking and negative marking is there\n");
            printf("-----------------------------------------------------------------------------------\n");
            fseek(question_bank,0,SEEK_SET);
            while (fread(&q, sizeof(q), 1, question_bank) == 1)   
            { 
                rcard.total_questions++;
                max_score+=q.marks;
            }
            fseek(question_bank,0,SEEK_SET);
            while (fread(&q, sizeof(q), 1, question_bank) == 1)   
            {   
                printf("\n*********************************************************************************\n");
                while(fgets(line,99,que_bank_txt)!=NULL)
                {
                    if(strcmp(line,"EOQ\n")==0)
                    break;
                    printf("%s",line);
                }
                printf("\nMaximum marks alloted to this question: %f\n",q.marks);
                if(q.question_type=='Q')
                {
                 printf("Partial Marks: %f for each correct option\n",q.partial_marks);
                printf("Negative Marks: %f for any wrong option\n",q.negative_marks);
                }
                printf("**********************************************************************************\n");
                printf("-----------------------------------------------------------------------------------\n");
                if(check_time(start_time,&timeup,timelimit)==1)   // for checking time up or not
                break;
                printf("\nDo you want to attempt this question? (Y or N)\n");
                  scanf(" %c",&attempt_choice);
                  if(attempt_choice=='N')
                  {
                    check_time(start_time,&timeup,timelimit); 
                    continue;
                  } 
                  printf("\nEnter your response(for MCQ and MSQ, in capital): \n");
                  if(q.question_type=='M')
                  { 
                       scanf(" %c",&mcq_response);
                       printf("\nCorrect answer to this question: %c\n",q.correct_option);
                  }
                  else if(q.question_type=='S')
                  {    
                      fflush(stdin);
                      fgets(subjective_response,99,stdin);
                      printf("\nCorrect answer to this question: %s\n",q.subjective_answer);
                  }
                  else if(q.question_type=='N')
                  {
                      scanf("%f",&numerical_response);
                      printf("\nCorrect answer to this question: %f\n",q.numerical_answer);
                  }
                  else if(q.question_type=='Q')
                  {  
                      counter=0;
                      while(msq_choice=='Y')
                      {   
                          counter++;
                          check=0;
                          printf("Enter your option:\n");
                          scanf(" %c",&msq_response);
                          if(check_time(start_time,&timeup,timelimit)==1)   // for checking time up or not
                           break;
                          for(int i=0;i<q.msq_correctoptsnumber;i++)
                          {
                              if(msq_response==q.msq_options[i])
                               { 
                                 rcard.total_score+=q.partial_marks;
                                 check=1;
                                 break;
                               } 
                               else
                               check=0;
                          }
                          if(check==0)
                          {   
                              rcard.total_score-=(counter-1)*(q.partial_marks);
                              rcard.total_score-=q.negative_marks;
                              printf("\nOne of the options entered is wrong!, so wrong answer.Marks deducted\n");
                              rcard.wrong_answers++;
                              break;
                          }
                          printf("\nWant to enter one more option? (Y or N)\n");
                          scanf(" %c",&msq_choice);
                          if(counter==q.msq_correctoptsnumber && msq_choice=='Y' && check==1)
                          {
                              rcard.total_score-=counter*(q.partial_marks); //all entered options were correct but an extra option is entered than required
                              rcard.total_score-=q.negative_marks;
                              printf("\nYou are trying to enter an extra option than the correct ones, so wrong answer.Marks deducted\n");
                              rcard.wrong_answers++;
                              break;
                          }
                            
                      }
                        
                        if(counter<q.msq_correctoptsnumber && check==1)
                        {
                            printf("\nPartial marks have been awarded for this question!\n");
                            rcard.partial_correct_answers++;
                        }    
                        else if(counter==q.msq_correctoptsnumber && check==1 && msq_choice=='N')
                        {  
                            rcard.total_score-=counter*(q.partial_marks);
                            rcard.total_score+=q.marks;
                           printf("\nFull marks have been awarded for this question!\n");
                           rcard.correct_answers++;
                        }
                        printf("\nCorrect option(s) to this question:\n");
                        for(int k=0;k<q.msq_correctoptsnumber;k++)
                        printf("%c ",q.msq_options[k]);
                  }

                  if(check_time(start_time,&timeup,timelimit)==1)   // for checking time up or not
                   break;
                  rcard.attempted_questions++;
                if(q.question_type!='Q')
                {
                  if((mcq_response==q.correct_option && q.question_type=='M') || (numerical_response==q.numerical_answer && q.question_type=='N') || (strcmp(subjective_response,q.subjective_answer)==0 && q.question_type=='S'))
                  {
                     rcard.correct_answers++;
                     printf("Correct answer !\n\n");
                     rcard.total_score+=q.marks;
                  }
                   else
                  {
                    rcard.wrong_answers++;
                    printf("Wrong answer !\n\n");
                  }
                } 
                 printf("\n----------------------------------------------------------------------------------\n\n");
                             
            } 
            fclose(que_bank_txt);
            time(&end_time);
            printf("\n===================================================================================");
           printf("\nThanks for attempting!\n");
           printf("Total number of  questions in quiz: %d\n",rcard.total_questions);
          printf("Total attempted questions: %d\n",rcard.attempted_questions);
           printf("Correct answers: %d\n",rcard.correct_answers);
           printf("Wrong answers: %d\n",rcard.wrong_answers);
           printf("Partial correct answers: %d\n",rcard.partial_correct_answers);
           printf("Maximum score for this quiz: %f\n",max_score);
           printf("Your total Score: %f\n",rcard.total_score);
             if(timeup==1)
             calculate_time(store_time,timelimit);
             else if(timeup==0)
             calculate_time(store_time,difftime(end_time,start_time));
          printf("Time taken to attempt the quiz: %d minutes %d seconds\n",store_time[0],store_time[1]);
           analytics_system(rcusrname,rcard.total_score,*flag);
          report_card(rcusrname,rcard,max_score,store_time);
           printf("\n==================================================================================\n\n");
           fclose(question_bank);
            printf("\nDo you want to view the quiz analytics?(Y/N) \n");
            scanf(" %c",&analytics_choice);
           if(analytics_choice=='Y')
           quiz_analytics();
}

/*
 * Function: report_card()
 * ---------------------------------------------------------------------------------------
 * rcusrname[100] will contain the username of the user, rcmaxscore will contain the maximum possible
   score for  the quiz and rcattempt_quiz will contain the user scorecard.All of this data is 
   received as arguements by report_card() function.              
*/

void report_card(char rcusrname[100],struct reportcard rcattempt_quiz,float rcmaxscore,int *storetime)
{
     FILE *report_file;      
     char user_report[100],analytics_choice;   
    sprintf(user_report, "%s.txt", rcusrname);
    report_file = fopen(user_report, "w");
    fprintf(report_file, "Username: %s\nTotal number of questions in quiz: %d\nTotal attempted questions: %d\n",rcusrname,rcattempt_quiz.total_questions,rcattempt_quiz.attempted_questions);
    fprintf(report_file,"Correct answers: %d\nWrong answers: %d\n",rcattempt_quiz.correct_answers,rcattempt_quiz.wrong_answers);
    fprintf(report_file,"Partial correct answers: %d\nMaximum possible score for the quiz: %f\n",rcattempt_quiz.partial_correct_answers,rcmaxscore);
    fprintf(report_file,"Your total Score: %f\n",rcattempt_quiz.total_score);
    fprintf(report_file,"Time taken to attempt the quiz: %d minutes %d seconds\n",storetime[0],storetime[1]);
    printf("Report card written successfully...\n");
    fclose(report_file);
}

/*
 * Function: calculate_time()
 * ---------------------------------------------------------------------------------------
 * calculates time in minutes and seconds, receives store_time[2] array and number of seconds
  (number) from attempt_quiz() function.                  
*/

void calculate_time(int *storetime,int number)   
{
   if(number<60)
    {
        storetime[0]=0;
        storetime[1]=number;
    }
     else
    {
      storetime[0]=(number)/60;
      storetime[1]=number-((storetime[0])*60);
    }
}

/*
 * Function: check_time()
 * ---------------------------------------------------------------------------------------
 * Receives start_time(passedtime), timeup(timeupornot) and time limit (tmlimit) as parameters
   from attempt_quiz() function.   
 * Checks whether time limit exceeded or not, value 1 is returned when time is up otherwise 
   value 0 is returned when time is still remaining.
 * timeupornot=1 implies that time is up while timeupornot=0 implies that time is still remaining             
*/

int check_time(time_t passedtime,int *timeupornot,int tmlimit) 
{
  time_t current_time;
  time(&current_time);    // stores the current system time in current_time variable
   if (difftime(current_time, passedtime) >= tmlimit) 
  {
     printf("\nSorry time is up!!\n");
     *timeupornot=1;
     return 1;
  } 
  else
    return 0;
}

/*
 * Function: collect_feedback()
 * ---------------------------------------------------------------------------------------
 * line[100] is used for storing a line read from the file by fgets() function
 * usercomment is a char array which will contain any comments from the user
 * fboptions[4] is an integer array which will contain the response for "Rate on a scale of 1-5"
   type questions.         
*/


void collect_feedback()                        
{
    FILE *feedback_form,*feedback_response;
    int i=0;
    char line[100],feedback_choice;
    feedback_form=fopen("Feedback_form.txt","r");
    feedback_response = fopen("Feedback_responses.DAT", "rb+");
    if (feedback_response == NULL)
    {
        feedback_response= fopen("Feedback_responses.DAT", "wb+");
        if (feedback_response == NULL)
        {
            puts("Cannot open file");
            exit(1);
        }
    }
    fseek(feedback_response, 0, SEEK_END);
    printf("\nYour feedback is valuable to us..Do you want to give your feedback?(Y/N)\n");
    scanf(" %c",&feedback_choice);
    if(feedback_choice=='Y')
  { 
    while(fgets(line,99,feedback_form)!=NULL && i<5)
    {  
       printf("\n*********************************************************************************\n");
       printf("%s",line);
       printf("Enter your reponse:\n");
       if(i==4)
       { 
        fflush(stdin);
        fgets(fb.usercomment,99,stdin);
       }
       else
       scanf("%d",&fb.fboptions[i]);
       printf("\n");
       i++;
       printf("\n*********************************************************************************\n");
    }
    fwrite(&fb, sizeof(fb), 1, feedback_response);
    printf("Thank you for giving your feedback!\n");
  } 
   fclose(feedback_form);
   fclose(feedback_response);
}

/*
 * Function: quiz_analytics()
 * ---------------------------------------------------------------------------------------
 * The array srt is sorted in ascending order on basis of total score. n=1 implies that only
   1 user has attempted the quiz till that time.
*/

void quiz_analytics()
{
   float lowest_score,highest_score,average_score=0.0;
   int count_lowest=1,count_highest=1,counter=0;
   char userhighest[100],userlowest[100];   
   /*
    userhighest[100] will store the username of user with highest score
    userlowest[100]  will store the username of user with lowest score
   */ 
   FILE *qz;        // file pointer qz will be used with file "Quiz_analytics.DAT"
   qz = fopen("Quiz_analytics.DAT", "rb+");
    if (qz == NULL)
    {
        qz = fopen("Quiz_analytics.DAT", "wb+");
        if (qz == NULL)
        {
            puts("Cannot open file");
            exit(1);
        }
    }
     fseek(qz,0, SEEK_END);
    long int file_size = ftell(qz);  // calculating the size of the file "Quiz_analytics.DAT"
    int s=sizeof(qa);    // s is size of 1 record
    int n=file_size/s ;   //no. of records 
    struct quizanalytics srt[n],temp;   // srt array will contain the user data stored in "Quiz_analytics.DAT" file
    fseek(qz,0,SEEK_SET);    // it will move the stream pointer to beginning of the file
    int i=0;
    while (fread(&qa, sizeof(qa), 1, qz) == 1)
    {
        if(i>=n)
        {
            break;
        }
        srt[i].tot_score=qa.tot_score;
        strcpy(srt[i].user_name,qa.user_name);
        average_score+=qa.tot_score;
        i++;
    }
   average_score=average_score/n ;         // calculating the average score
   fclose(qz);
   for (int i = 1; i < n; i++)          //sorting srt array using bubble sort
      for (int j = 0; j < n - i; j++) {
         if (srt[j].tot_score>srt[j+1].tot_score) {
            temp = srt[j];
            srt[j] = srt[j + 1];
            srt[j + 1] = temp;
         }
      }
    for(int i=1;i<n-1;i++)       // counting no.of users with lowest and highest score
     {
       if(srt[i].tot_score==srt[0].tot_score)   
        count_lowest++;
        else if(srt[n-1-i].tot_score==srt[n-1].tot_score)   
        count_highest++;
     }  
      lowest_score=srt[0].tot_score;
      highest_score=srt[n-1].tot_score;
     printf("********************************************************************\n");
     printf("Welcome to QUIZ ANALYTICS !!\nFollowing data is for users who have taken test till now\n");
     if(n!=1)
     {     
           printf("\nNo. of users who have attempted the quiz: %d\n",n);
           printf("\nLowest score till now is %f achieved by :\n",lowest_score);
         for(int i=0;i<count_lowest;i++)
        {
          printf("%d.%s\n",i+1,srt[i].user_name);
        }
          printf("\nHighest score till now is %f achieved by:\n",highest_score);
        for(int i=n-1;i>0;i--)
       {  
          counter++;
          if(counter>count_highest)
            break;
          printf("%d.%s\n",n-i,srt[i].user_name);
       }
         printf("\nAverage score for the quiz is %f\n",average_score);
     }
     else
        printf("Only 1 user has attempted the quiz till now\nMarks scored: %f by %s\n",srt[0].tot_score,srt[0].user_name);
        printf("********************************************************************\n");

}
/*
 * Function: analytics_system()
 * ---------------------------------------------------------------------------------------
 * asusrname will store the username of the user and astotscore will contain the total score 
   of the user  passed to analytics_system() by attempt_quiz() function
 * flag_value=0 implies that it is the first attempt of the user while flag_value=1 implies
   that it is the re-attempt(second attempt) of the user.                 
*/

void analytics_system(char asusrname[100],float astotscore,int flag_value)
{
     FILE *quizats,*temp;
     quizats = fopen("Quiz_analytics.DAT", "rb+");
     if(flag_value==0)
     {
      if (quizats == NULL)
      {
        quizats = fopen("Quiz_analytics.DAT", "wb+");
        if (quizats == NULL)
        {
            puts("Cannot open file");
            exit(1);
        }
      }
       fseek(quizats,0,SEEK_END);
       strcpy(qa.user_name,asusrname);
       qa.tot_score=astotscore;
       fwrite(&qa, sizeof(qa), 1, quizats);
       fclose(quizats);
       printf("Data successfully sent to our analytics system\n");
     }
     else if(flag_value==1)
     {
         temp=fopen("temptest.DAT","wb");
         fseek(quizats,0,SEEK_SET);//rewind(quizats);
         while ( fread ( &qa, sizeof(qa), 1, quizats ) == 1 )
         {
           if ( strcmp ( qa.user_name, asusrname ) != 0 )
            fwrite ( &qa, sizeof(qa), 1, temp ) ;
         }
         strcpy(qa.user_name,asusrname);
         qa.tot_score=astotscore;
         fwrite(&qa, sizeof(qa), 1, temp);
         fclose(quizats);
         fclose(temp);
         remove("Quiz_analytics.DAT");
         rename("temptest.DAT","Quiz_analytics.DAT");
         printf("Data successfully sent to our analytics system\n");
     }  

}

/*
 * Function: create_feedback()
 * ---------------------------------------------------------------------------------------
 * cf is a file pointer which will be used with the file "feedback_form.txt", line[100] stores
   the line read by fgets() function.            
*/

void create_feedback()
{
   FILE *cf;
   cf=fopen("Feedback_form.txt","w");
   char line[100];
   printf("\nEnter 5 questions for feedback(keep last question as comment)\n");
   for(int i=1;i<=5;i++)
   {
       printf("Enter question no. %d :\n",i);
       fflush(stdin);
       fgets(line,99,stdin);
       fprintf(cf,"%s",line);
   }
   printf("Thank you for creating the feedback form\n");
   fclose(cf);
}

/*
 * Function: view_feedback()
 * ---------------------------------------------------------------------------------------
 * The array count will store the no. of responses for a particular option (1 to 5) and sum
   will contain the sum of all counts for that particular question.
 * The array percentage will contain the percentage of users who have selected that particular
   option for that particular question.        
*/

void view_feedback()
{
    FILE *vf,*feedback_response;
    char user_comment[100],line[100];
    int count[5]={0,0,0,0,0},sum;
    float percentage[5]={0.0,0.0,0.0,0.0,0.0};
    vf=fopen("Feedback_form.txt","r");
    feedback_response = fopen("Feedback_responses.DAT", "rb+");
    if (feedback_response == NULL)
    {
        feedback_response= fopen("Feedback_responses.DAT", "wb+");
        if (feedback_response == NULL)
        {
            puts("Cannot open file");
            exit(1);
        }
    }
    fseek(feedback_response,0,SEEK_SET);
    printf("\nWelcome to feedback! Feedback is as follows:\n");
    for(int i=0;i<5;i++)
    {     
         sum=0;
         if(i==4)
         {   
             fgets(line,99,vf);
             printf("\nFor question : %s\n",line);
             printf("User comments are as follows:\n\n");
             while ( fread ( &fb, sizeof(fb), 1, feedback_response ) == 1 )
            {
                printf("%s\n",fb.usercomment);
            }
         }
         else
         {
            while ( fread ( &fb, sizeof(fb), 1, feedback_response ) == 1 )
          {
           if(fb.fboptions[i]==1)
           count[0]++;
           else if(fb.fboptions[i]==2)
           count[1]++;
           else if(fb.fboptions[i]==3)
           count[2]++;
           else if(fb.fboptions[i]==4)
           count[3]++;
           else if(fb.fboptions[i]==5)
           count[4]++;
          }
          sum=count[0]+count[1]+count[2]+count[3]+count[4];
           for(int j=0;j<5;j++)
            percentage[j]=((float)count[j]/(float)sum)*100;
            fgets(line,99,vf);
            printf("\nFor question : %s\n",line);
            for(int j=0;j<5;j++)
            printf("Percentage of users selected %d : %f\n",j+1,percentage[j]);
           fseek(feedback_response,0,SEEK_SET);//rewind(feedback_response);
          for(int j=0;j<5;j++)
          {
            count[j]=0;
            percentage[j]=0.0;
          }

         }

    }
    printf("\nAll feedback received till now has been displayed..\n");
    fclose(vf);
    fclose(feedback_response);
}