
import java.util.ArrayList;
import java.util.Date;



  public class SeqIc {
	  
	   int getLen(String s){
		      if(s==null)
		         return 1;
		      return s.length()+2;

		    }

	  
	  
	  void DP4(ArrayList str){
			 int kval1 = 0, kval2=0, kval3=0, kval4=0, k11, k21, k31, k41;
			 char []X=new char[getLen((String)str.get(0))]; char []Y=new char[getLen((String)str.get(1))];
			 X=((String)str.get(0)).toCharArray(); Y=((String)str.get(1)).toCharArray();
			 char []P1=new char[getLen((String)str.get(2))];   P1=((String)str.get(2)).toCharArray();
			 char []P2=new char[getLen((String)str.get(3))];   P2=((String)str.get(3)).toCharArray();
			 char []P3=new char[getLen((String)str.get(4))];   P3=((String)str.get(4)).toCharArray();
			 char []P4=new char[getLen((String)str.get(5))];    P4=((String)str.get(5)).toCharArray();
			 
			 int n1 =((String)str.get(0)).length(),
			n2=((String)str.get(1)).length(), 
			m1=((String)str.get(2)).length(), 
			m2=((String)str.get(3)).length(),
			m3=((String)str.get(4)).length(), 
			m4=((String)str.get(5)).length(), k1, k2, k3, k4, i, j; 
			 
			 short D[][][][][][]=new short [n1+1][n2+1][m1+1][m2+1][m3+1][m4+1];

			 for(i=0;i<=n1;i++){
					// D[i][0][0][0]=0;
					 for(k1=0;k1<=m1;k1++)
						 for(k2=0;k2<=m2;k2++)
						  for(k3=0;k3<=m3;k3++)
							  for(k4=0;k4<=m4;k4++)
						 {
							 D[i][0][k1][k2][k3][k4]=-9999;
						 }
				 }
				
				 
				 
				 for(j=0;j<=n2;j++){
					// D[0][j][0][0]=0;
					 for(k1=0;k1<=m1;k1++){
						 for(k2=0;k2<=m2;k2++){
						   for(k3=0;k3<=m3;k3++)
							  for(k4=0;k4<=m4;k4++)
						 {
							 D[0][j][k1][k2][k3][k4]=-9999;
						 }
					   }
					}
				 }
				 
				 for(j=0;j<=n2;j++)
					 D[0][j][0][0][0][0]=0;
				 for(i=0;i<=n1;i++)
					 D[i][0][0][0][0][0]=0;
			 
			 long startTime1 = new Date().getTime();
		 
			 char ch;
			/* for(i=0;i<=n1;i++)
			  for(j=0;j<=n2;j++)
				for(k1=0;k1<=m1;k1++){
					 
					for(k2=0;k2<=m2;k2++){
						
						for(k3=0;k3<=m3;k3++){
							
							for(k4=0;k4<=m4;k4++){
								kval4=k4; kval1=k1; kval2=k2; kval3=k3;
	
							if((i>0 && j>0) &&(((String)str.get(0)).charAt(i-1)!=((String)str.get(1)).charAt(j-1)))
								D[i][j][k1][k2][k3][k4]=(short)Math.max(D[i-1][j][k1][k2][k3][k4], D[i][j-1][k1][k2][k3][k4]);
								else if((i>0 && j>0) &&(((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1)))
                                {
									 ch=((String)str.get(0)).charAt(i-1);
									if((i>0 && k1>0) && (ch==P1[k1-1]))kval1=k1-1;
									if((i>0 && k2>0) && (ch==P2[k2-1]))kval2=k2-1;
									if((i>0 && k3>0) && (ch==P3[k3-1]))kval3=k3-1;
									if((i>0 && k4>0) && (ch==P4[k4-1]))kval4=k4-1;
									
									D[i][j][k1][k2][k3][k4]=(short)(1+D[i][j][kval1][kval2][kval3][kval4]);
								}
							}
						}
					}
				}
				*/
			 for(i=0;i<=((String)str.get(0)).length();i++)
					
			 {
				for(j=0;j<=((String)str.get(1)).length();j++)
				 {
			    	for(k1=0;k1<=((String)str.get(2)).length();k1++)
				  {
					 for(k2=0;k2<=((String)str.get(3)).length();k2++)
					  {
						 for(k3=0;k3<=((String)str.get(4)).length();k3++){
							 for(k4=0;k4<=((String)str.get(5)).length();k4++)
						   {
						
						  if((i>0 && j>0) &&(((String)str.get(0)).charAt(i-1)!=((String)str.get(1)).charAt(j-1)))
							D[i][j][k1][k2][k3][k4]=(short)Math.max(D[i-1][j][k1][k2][k3][k4], D[i][j-1][k1][k2][k3][k4]);
						else if((i>0 && j>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1))){
							
						       kval1=k1; kval2=k2; kval3=k3; kval4=k4;
								
						       
						       if(((i>0 && k1>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(2)).charAt(k1-1)))) kval1=k1-1;
							   if(((i>0 && k2>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(3)).charAt(k2-1)))) kval2=k2-1;
							   if(((i>0 && k3>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(4)).charAt(k3-1)))) kval3=k3-1;
							   if(((i>0 && k4>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(4)).charAt(k4-1)))) kval4=k4-1;
							   D[i][j][k1][k2][k3][k4]=(short)(1+D[i-1][j-1][kval1][kval2][kval3][kval4]);
							}
					  }	
			       }	
				
				}
			  }  
			}
		}
			 long endTime1 = new Date().getTime();
	         System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));
	         System.out.println("Total length of DP CLCS  is :"+ D[n1][n2][m1][m2][m3][m4]);
	
	  
	  }

	 
	  void DP2(ArrayList str){
			 char []X=new char[getLen((String)str.get(0))]; char []Y=new char[getLen((String)str.get(1))];
			 	
		     X=((String)str.get(0)).toCharArray(); Y=((String)str.get(1)).toCharArray();
			 
			 char []P1=new char[getLen((String)str.get(2))];   P1=((String)str.get(2)).toCharArray();
			 char []P2=new char[getLen((String)str.get(3))];   P2=((String)str.get(3)).toCharArray();
			 int kval1 = 0, kval2=0, kval3=0, kval4=0, k11, k21, k31, k41;
			 int n1 =((String)str.get(0)).length(), 
			n2=((String)str.get(1)).length(),
			m1=((String)str.get(2)).length(), 
			m2=((String)str.get(3)).length(),  k1, k2, i, j; 
		     short D[][][][]=new short [n1+2][n2+2][m1+2][m2+2];
			 long startTime1 = new Date().getTime();
             
		
			 
			 for(i=0;i<=n1;i++){
				// D[i][0][0][0]=0;
				 for(k1=0;k1<=m1;k1++)
					 for(k2=0;k2<=m2;k2++){
						 D[i][0][k1][k2]=-9999;
					 }
			 }
			
			 
			 
			 for(j=0;j<=n2;j++){
				// D[0][j][0][0]=0;
				 for(k1=0;k1<=m1;k1++){
					 for(k2=0;k2<=m2;k2++){
						 D[0][j][k1][k2]=-9999;
					 }
				}
			 }
			 
			 for(j=0;j<=n2;j++)
				 D[0][j][0][0]=0;
			 for(i=0;i<=n1;i++)
				 D[i][0][0][0]=0;
			 
			 
			 for(i=0;i<=((String)str.get(0)).length();i++)
			
			 {
				
				
				for(j=0;j<=((String)str.get(1)).length();j++)
				 {
					
					for(k1=0;k1<=((String)str.get(2)).length();k1++)
				  {
					 
					
						for(k2=0;k2<=((String)str.get(3)).length();k2++){
						
						if((i>0 && j>0) &&(((String)str.get(0)).charAt(i-1)!=((String)str.get(1)).charAt(j-1)))
							D[i][j][k1][k2]=(short)Math.max(D[i-1][j][k1][k2], D[i][j-1][k1][k2]);
						else if((i>0 && j>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1))){
							
						       kval1=k1; kval2=k2;
								
						       
						       if((k1>0 && (((String)str.get(0)).charAt(i-1)==((String)str.get(2)).charAt(k1-1)))) kval1=k1-1;
							   if((k2>0 && (((String)str.get(0)).charAt(i-1)==((String)str.get(3)).charAt(k2-1)))) kval2=k2-1;
							   D[i][j][k1][k2]=(short)(1+D[i-1][j-1][kval1][kval2]);
							}
					  }	
			    }	
			 
				  
			}
		}
			 long endTime1 = new Date().getTime();
	         System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));
	         System.out.println("Total length of DP CLCS  is :"+ D[n1][n2][m1][m2]);

	  
	  }
  
	  
	  void DP3(ArrayList str){
			 char []X=new char[getLen((String)str.get(0))]; char []Y=new char[getLen((String)str.get(1))];
			 	
		     X=((String)str.get(0)).toCharArray(); Y=((String)str.get(1)).toCharArray();
			 
			 char []P1=new char[getLen((String)str.get(2))];   P1=((String)str.get(2)).toCharArray();
			 char []P2=new char[getLen((String)str.get(3))];   P2=((String)str.get(3)).toCharArray();
			 int kval1 = 0, kval2=0, kval3=0, kval4=0, k11, k21, k31, k41;
			 int n1 =((String)str.get(0)).length(), 
			n2=((String)str.get(1)).length(),
			m1=((String)str.get(2)).length(), 
			m2=((String)str.get(3)).length(), 
			m3=((String)str.get(4)).length(),
			k1, k2, k3, i, j; 
		     short D[][][][][]=new short [n1+2][n2+2][m1+2][m2+2][m3+2];
			 long startTime1 = new Date().getTime();
          
		
			 
			 for(i=0;i<=n1;i++){
				// D[i][0][0][0]=0;
				 for(k1=0;k1<=m1;k1++)
					 for(k2=0;k2<=m2;k2++)
					  for(k3=0;k3<=m3;k3++)
					 {
						 D[i][0][k1][k2][k3]=-9999;
					 }
			 }
			
			 
			 
			 for(j=0;j<=n2;j++){
				// D[0][j][0][0]=0;
				 for(k1=0;k1<=m1;k1++){
					 for(k2=0;k2<=m2;k2++){
					   for(k3=0;k3<=m3;k3++)
					 {
						 D[0][j][k1][k2][k3]=-9999;
					 }
				   }
				}
			 }
			 
			 for(j=0;j<=n2;j++)
				 D[0][j][0][0][0]=0;
			 for(i=0;i<=n1;i++)
				 D[i][0][0][0][0]=0;
			 
			 
			 for(i=0;i<=((String)str.get(0)).length();i++)
			
			 {
				
				
				for(j=0;j<=((String)str.get(1)).length();j++)
				 {
					
					for(k1=0;k1<=((String)str.get(2)).length();k1++)
				  {
					 
					
						for(k2=0;k2<=((String)str.get(3)).length();k2++){
							for(k3=0;k3<=((String)str.get(4)).length();k3++)
						 {
						
						if((i>0 && j>0) &&(((String)str.get(0)).charAt(i-1)!=((String)str.get(1)).charAt(j-1)))
							D[i][j][k1][k2][k3]=(short)Math.max(D[i-1][j][k1][k2][k3], D[i][j-1][k1][k2][k3]);
						else if((i>0 && j>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1))){
							
						       kval1=k1; kval2=k2; kval3=k3;
								
						       
						       if((k1>0 && (((String)str.get(0)).charAt(i-1)==((String)str.get(2)).charAt(k1-1)))) kval1=k1-1;
							   if((k2>0 && (((String)str.get(0)).charAt(i-1)==((String)str.get(3)).charAt(k2-1)))) kval2=k2-1;
							   if((k3>0 && (((String)str.get(0)).charAt(i-1)==((String)str.get(4)).charAt(k3-1)))) kval3=k3-1;
							   D[i][j][k1][k2][k3]=(short)(1+D[i-1][j-1][kval1][kval2][kval3]);
							}
					  }	
			    }	
			 
			  }  
			}
		}
			 long endTime1 = new Date().getTime();
	         System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));
	         System.out.println("Total length of DP CLCS  is :"+ D[n1][n2][m1][m2][m3]);

	  
	  }

	  
	  
	  
	  void DP1(ArrayList str){
			 char []X=new char[getLen((String)str.get(0))]; char []Y=new char[getLen((String)str.get(1))];
			 	
		     X=((String)str.get(0)).toCharArray(); Y=((String)str.get(1)).toCharArray();
			 
			 char []P1=new char[getLen((String)str.get(2))];   P1=((String)str.get(2)).toCharArray();
			
			 int kval1 = 0, kval2=0, kval3=0, kval4=0, k11, k21, k31, k41;
			 
			 int n1 =((String)str.get(0)).length(),
			 n2=((String)str.get(1)).length(), 
			 m1=((String)str.get(2)).length(),  k1, k2, i, j; 
		     
			 short D[][][]=new short [n1+2][n2+2][m1+2];
			 long startTime1 = new Date().getTime();
          
			 for(j=0;j<=n2;j++){
				
				 D[0][j][0]=0;
				 for(k1=1;k1<=m1;k1++)
				 {
						 D[0][j][k1]=-9999;
					 }
				 
			 }
			
			 for(i=0;i<=n1;i++){
				 
				 D[i][0][0]=0;
				 for(k1=1;k1<=m1;k1++)
					 {
						 D[i][0][k1]=-9999;
					 }
			 }
			 
			 for(i=0;i<=n1;i++){
			  for(j=0;j<=n2;j++){
				for(k1=0;k1<=m1;k1++){
					 kval1=k1;
					if((i>0 && j>0) &&(((String)str.get(0)).charAt(i-1)!=((String)str.get(1)).charAt(j-1)))
							D[i][j][k1]=(short)Math.max(D[i-1][j][k1], D[i][j-1][k1]);
						else if((i>0 && j>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1))){
							
								if((i>0 && k1>0) && (((String)str.get(0)).charAt(i-1)==((String)str.get(2)).charAt(k1-1)))
								{
									kval1=k1-1;
								}
							        D[i][j][k1]=(short)(1+D[i-1][j-1][kval1]);
							    
								//else if((k1==0) ||(((String)str.get(0)).charAt(i-1)!=((String)str.get(2)).charAt(k1-1)))
								{
									
							      //  D[i][j][k1]=(short)(1+D[i-1][j-1][k1]);
							    }
							}
					  
			    }	
			 
			}
		}
			
			/* for(i=0;i<=n1;i++){
				  for(j=0;j<=n2;j++){
					for(k1=0;k1<=m1;k1++){
						
					  if((i>0 && j>0) && (k1>0 && (X[i-1]==P1[k1-1])) &&(X[i-1]==Y[j-1]) )
						  D[i][j][k1]=(short)(1+D[i-1][j-1][k1-1]);
					 
					   if((i>0 && j>0) &&(X[i-1]==Y[j-1]) && (k1==0|| (X[i-1]!=P1[k1-1])))
						  D[i][j][k1]=(short)(1+D[i-1][j-1][k1]);
					 
					   if((i>0 && j>0) &&(X[i-1]!=Y[j-1]))
						  D[i][j][k1]=(short)Math.max(D[i-1][j][k1], D[i][j-1][k1]);
					 }	 
					
				  }
			 }*/
			 long endTime1 = new Date().getTime();
	         System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));
	         System.out.println("Total length of DP CLCS  is :"+ D[n1][n2][m1]);

	  
	  }

	  
	  
	  
  }