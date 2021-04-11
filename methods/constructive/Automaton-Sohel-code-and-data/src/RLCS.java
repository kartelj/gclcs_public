

import java.util.*;

public class RLCS{

    int getLen(String s){
      if(s==null)
         return 1;
      return s.length()+2;

    }

    void DPmethod(ArrayList str){


        int dp[][][][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))][getLen((String)str.get(5))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                        for(int k1=1;k1<=((String)str.get(5)).length();k1++)
                       {
                              max=Math.max(dp[i-1][j][j1][j2][k][k1], dp[i][j-1][j1][j2][k][k1]);
                              max=Math.max(max, dp[i][j][j1-1][j2][k][k1]);
                              max=Math.max(max, dp[i][j][j1][j2-1][k][k1]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(j1-1))&&(ch==((String)str.get(3)).charAt(j2-1)))
                             {
                                if((ch==((String)str.get(4)).charAt(k-1))&&(ch==((String)str.get(5)).charAt(k1-1)))
                                    dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1][k1-1]+1);
                                else
                                   dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k][k1]+1);
                             }
                           else
                               dp[i][j][j1][j2][k][k1]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()][((String)str.get(5)).length()]);
        System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

}

      void DPmethod51(ArrayList str){


        int dp[][][][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))][getLen((String)str.get(5))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                        for(int k1=1;k1<=((String)str.get(5)).length();k1++)
                       {
                              max=Math.max(dp[i-1][j][j1][j2][k][k1], dp[i][j-1][j1][j2][k][k1]);
                              max=Math.max(max, dp[i][j][j1-1][j2][k][k1]);
                              max=Math.max(max, dp[i][j][j1][j2-1][k][k1]);
                              max=Math.max(max, dp[i][j][j1][j2][k-1][k1]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(j1-1))&&(ch==((String)str.get(3)).charAt(j2-1))&&(ch==((String)str.get(4)).charAt(k-1)))
                             {
                                if((ch==((String)str.get(5)).charAt(k1-1)))
                                    dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1][k1-1]+1);
                                else
                                   dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1][k1]+1);
                             }
                           else
                               dp[i][j][j1][j2][k][k1]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()][((String)str.get(5)).length()]);
        System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

}

    void DPmethod33(ArrayList str){


        int dp[][][][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))][getLen((String)str.get(5))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                        for(int k1=1;k1<=((String)str.get(5)).length();k1++)
                       {
                              max=Math.max(dp[i-1][j][j1][j2][k][k1], dp[i][j-1][j1][j2][k][k1]);
                              max=Math.max(max, dp[i][j][j1-1][j2][k][k1]);
                             // max=Math.max(max, dp[i][j][j1][j2-1][k][k1]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(j1-1)))
                             {
                                if((ch==((String)str.get(4)).charAt(k-1))&&(ch==((String)str.get(5)).charAt(k1-1))&&(ch==((String)str.get(3)).charAt(j2-1)))
                                    dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1][k1-1]+1);
                                else
                                   dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k][k1]+1);
                             }
                           else
                               dp[i][j][j1][j2][k][k1]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()][((String)str.get(5)).length()]);
        System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

}

     void DPmethod62(ArrayList str){


        int dp[][][][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))][getLen((String)str.get(5))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                        for(int k1=1;k1<=((String)str.get(5)).length();k1++)
                       {
                              max=Math.max(dp[i-1][j][j1][j2][k][k1], dp[i][j-1][j1][j2][k][k1]);
                             // max=Math.max(max, dp[i][j][j1-1][j2][k][k1]);
                              //max=Math.max(max, dp[i][j][j1][j2-1][k][k1]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1)))
                             {
                                if((ch==((String)str.get(4)).charAt(k-1))&&(ch==((String)str.get(5)).charAt(k1-1))&&(ch==((String)str.get(2)).charAt(j1-1))&&(ch==((String)str.get(3)).charAt(j2-1)))
                                    dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1][k1-1]+1);
                                else
                                   dp[i][j][j1][j2][k][k1]=Math.max(max,dp[i-1][j-1][j1][j2][k][k1]+1);
                             }
                           else
                               dp[i][j][j1][j2][k][k1]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()][((String)str.get(5)).length()]);
        System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

}

    void DPmethod2(ArrayList str){


        int dp[][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int k=1;k<=((String)str.get(2)).length();k++)
                        {
                              max=Math.max(dp[i-1][j][k], dp[i][j-1][k]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1)))
                             {
                                if((ch==((String)str.get(2)).charAt(k-1)))
                                    dp[i][j][k]=Math.max(max,dp[i-1][j-1][k-1]+1);
                                else
                                   dp[i][j][k]=Math.max(max,dp[i-1][j-1][k]+1);
                             }
                           else
                               dp[i][j][k]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP2 "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()]);
        System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

}


    void DPmethod3(ArrayList str){


        int dp[][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))];

        long startTime1 = new Date().getTime();
        int max = -999, kval1, kval2;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                      for(int k=1;k<=((String)str.get(2)).length();k++)
                        for(int k1=1;k1<=((String)str.get(3)).length();k1++)
                       {
                              max=Math.max(dp[i-1][j][k][k1], dp[i][j-1][k][k1]);
                               ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1)))
                             {  
                                  kval1=k;kval2=k1;
                                  if((ch==((String)str.get(2)).charAt(k-1)))kval1=k-1;
                                  if((ch==((String)str.get(3)).charAt(k1-1)))kval2=k1-1;
                                  
                            	 if((ch==((String)str.get(2)).charAt(k-1))&&(ch==((String)str.get(3)).charAt(k1-1)))
                                    dp[i][j][k][k1]=Math.max(max,dp[i-1][j-1][k-1][k1-1]);
                                else
                                   dp[i][j][k][k1]=Math.max(max,dp[i-1][j-1][kval1][kval2]+1);

                             }
                           else
                               dp[i][j][k][k1]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()]);
        System.out.println("Total elapsed time in execution of DP3 CLCS  is :"+ (endTime1-startTime1));

}

    void DPmethod4(ArrayList str)//L-3 l-1
    {

        // long startTime1 = new Date().getTime();
        int dp[][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                      for(int k=1;k<=((String)str.get(2)).length();k++)
                        for(int k1=0;k1<=((String)str.get(3)).length();k1++)
                       {
                              max=Math.max(dp[i-1][j][k][k1], dp[i][j-1][k][k1]);
                              max=Math.max(max, dp[i][j][k-1][k1]);
                               ch=((String)str.get(0)).charAt(i-1);
                              if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(k-1)))
                             {
                                if(k1>0 &&(ch==((String)str.get(3)).charAt(k1-1))){
                                    
                                	 int max1=Math.max(dp[i-1][j-1][k-1][k1],dp[i-1][j-1][k-1][k1-1]);
                                	 dp[i][j][k][k1]=Math.max(max,max1);
                                    }
                                else
                                   dp[i][j][k][k1]=Math.max(max,dp[i-1][j-1][k-1][k1]+1);
                             }
                           else
                               dp[i][j][k][k1]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP pp "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()]);
        System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

}

   void DPmethod5(ArrayList str){


        short dp[][][][][]=new short[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                       {      max=Math.max(dp[i-1][j][j1][j2][k], dp[i][j-1][j1][j2][k]);
                              max=Math.max(max, dp[i][j][j1-1][j2][k]);
                              max=Math.max(max, dp[i][j][j1][j2-1][k]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(j1-1))&&(ch==((String)str.get(3)).charAt(j2-1)))
                             {
                                if((ch==((String)str.get(4)).charAt(k-1)))
                                    dp[i][j][j1][j2][k]=(short)Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1]+1);
                                else
                                   dp[i][j][j1][j2][k]=(short)Math.max(max,dp[i-1][j-1][j1-1][j2-1][k]+1);
                             }
                           else
                               dp[i][j][j1][j2][k]=(short)max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()]);
        System.out.println("Total elapsed time in execution of DP5 CLCS  is :"+ (endTime1-startTime1));

}



   void DPmethod5_23(ArrayList str){


        int dp[][][][][]=new int[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))];

        long startTime1 = new Date().getTime();
        int max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                       {      max=Math.max(dp[i-1][j][j1][j2][k], dp[i][j-1][j1][j2][k]);
                              //max=Math.max(max, dp[i][j][j1-1][j2][k]);
                              //max=Math.max(max, dp[i][j][j1][j2-1][k]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1)))
                             {
                                if((ch==((String)str.get(4)).charAt(k-1))&&(ch==((String)str.get(3)).charAt(j2-1))&&(ch==((String)str.get(2)).charAt(j1-1)))
                                    dp[i][j][j1][j2][k]=Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1]+1);
                                else
                                   dp[i][j][j1][j2][k]=Math.max(max,dp[i-1][j-1][j1][j2][k]+1);
                             }
                           else
                               dp[i][j][j1][j2][k]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()]);
        System.out.println("Total elapsed time in execution of DP5 CLCS  is :"+ (endTime1-startTime1));

}

   void DPmethod5_32(ArrayList str){


        short dp[][][][][]=new short[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))];

        long startTime1 = new Date().getTime();
        int max = -999,kval1,kval2;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                       {      max=Math.max(dp[i-1][j][j1][j2][k], dp[i][j-1][j1][j2][k]);
                              max=Math.max(max, dp[i][j][j1-1][j2][k]);
                              //max=Math.max(max, dp[i][j][j1][j2-1][k]);
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(j1-1)))
                             {
                            	 kval1=j2; kval2=k;
                            	 if((ch==((String)str.get(4)).charAt(k-1))) kval2=k-1;
                            	 if((ch==((String)str.get(3)).charAt(j2-1))) kval1=j2-1;
                            	 if((kval1==j2-1) && (kval2==k-1))
                                    dp[i][j][j1][j2][k]=(short)Math.max(max,dp[i-1][j-1][j1-1][j2-1][k-1]);
                                else
                                   dp[i][j][j1][j2][k]=(short)Math.max(max,dp[i-1][j-1][j1-1][kval1][kval2]+1);
                             }
                           else
                               dp[i][j][j1][j2][k]=(short)max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()]);
        System.out.println("Total elapsed time in execution of DP5 CLCS  is :"+ (endTime1-startTime1));

}

   void DPmethod5_41(ArrayList str){


        short dp[][][][][]=new short[getLen((String)str.get(0))][getLen((String)str.get(1))][getLen((String)str.get(2))][getLen((String)str.get(3))][getLen((String)str.get(4))];

        long startTime1 = new Date().getTime();
        short max = -999;
        char ch;
        for(int i=1;i<=((String)str.get(0)).length();i++)
            for(int j=1;j<=((String)str.get(1)).length();j++)
                for(int j1=1;j1<=((String)str.get(2)).length();j1++)
                    for(int j2=1;j2<=((String)str.get(3)).length();j2++)
                      for(int k=1;k<=((String)str.get(4)).length();k++)
                       {     // max=Math.max(dp[i-1][j][j1][j2][k], dp[i][j-1][j1][j2][k]);
                             // max=Math.max(max, dp[i][j][j1-1][j2][k]);
                             // max=Math.max(max, dp[i][j][j1][j2-1][k]);
                              
                              max=dp[i-1][j][j1][j2][k]>dp[i][j-1][j1][j2][k]?dp[i-1][j][j1][j2][k]:dp[i][j-1][j1][j2][k];
                              max=max>dp[i][j][j1-1][j2][k]?max:dp[i][j][j1-1][j2][k];
                              max=max>dp[i][j][j1][j2-1][k]?max:dp[i][j][j1][j2-1][k];
                              ch=((String)str.get(0)).charAt(i-1);
                             if((ch==((String)str.get(1)).charAt(j-1))&&(ch==((String)str.get(2)).charAt(j1-1))&&(ch==((String)str.get(3)).charAt(j2-1)))
                             {
                                if((ch==((String)str.get(4)).charAt(k-1)))
                                    dp[i][j][j1][j2][k]=(short) (max>dp[i-1][j-1][j1-1][j2-1][k-1]+1?max:dp[i-1][j-1][j1-1][j2-1][k-1]+1);
                                else
                                   dp[i][j][j1][j2][k]=(short)Math.max(max,dp[i-1][j-1][j1-1][j2-1][k]+1);
                             }
                           else
                               dp[i][j][j1][j2][k]=max;
                     }
       long endTime1 = new Date().getTime();

        System.out.println("length by DP "+dp[((String)str.get(0)).length()][((String)str.get(1)).length()][((String)str.get(2)).length()][((String)str.get(3)).length()][((String)str.get(4)).length()]);
        System.out.println("Total elapsed time in execution of DP5 CLCS  is :"+ (endTime1-startTime1));

}

}
