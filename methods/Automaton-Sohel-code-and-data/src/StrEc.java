

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

public class StrEc {

	public static final int sigma = 20;
	int f[][]=new int[4][100];
	
	
	int next[][][]=new int[4][100][sigma];
    char [] mySigma={'A', 'C', 'G','T','D', 'E', 'F', 'H','I','K','L','M','N','P','Q','R','S','V','W','Y'};
    HashMap index=new HashMap();
    ArrayList str;
    short D[][][];
    short D1[][][][];
    short D2[][][][][];
    byte D3[][][][][][];
    
	void Prefix(String P, int count){
		int m, q, k;
		m=P.length();
		f[count][1]=0;
		k=0;
		for(q=2;q<=m;q++){
			while(k>0 &&(P.charAt(k)!=P.charAt(q-1)))
				k=f[count][k];
			if(P.charAt(k)==P.charAt(q-1))
				k=k+1;
			f[count][q]=k;
		}
	
		
	}
	
	void ComputeNext(String S, int count){
		int i,j;
		char s;
	
		
		s=S.charAt(0);
		
		for(i=0;i<sigma;i++)
		{
			if(mySigma[i]!=s)
				next[count][0][(Integer)index.get(mySigma[i])]=0;
		}
	
	    next[count][0][(Integer)index.get(s)]=1;
	    
	    for(i=1;i<S.length();i++)
	    {
	    	for(j=0;j<sigma;j++){
	    		if(mySigma[j]==S.charAt(i))
	    			next[count][i][(Integer)index.get(mySigma[j])]=i+1;
	    		else
	    			next[count][i][(Integer)index.get(mySigma[j])]=next[count][f[count][i]][(Integer)index.get(mySigma[j])];
	    	}
	    }
	}
	
    short L(int i, int j, int l){
					
		if(D[i][j][l]!=-1)
			return D[i][j][l];
		
    	if(l==((String)str.get(2)).length())
			return D[i][j][l]=(short)-999;
		else if(i==0 || j==0)
			return D[i][j][l]=0;
		else if(((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1)){
			char c=((String)str.get(0)).charAt(i-1);
			int l1=next[0][l][(Integer)index.get(c)], temp;
			temp=Math.max(L(i-1,j,l), L(i,j-1,l));
			return D[i][j][l]=(short)Math.max(L(i-1,j-1,l1)+1, temp);
		}
		else
			return D[i][j][l]=(short)Math.max(L(i-1,j,l), L(i,j-1,l));
	}

    
	
	
	void StrAlgo1(ArrayList input){
		
		long startTime1 = new Date().getTime();
		str=input;
		String P=(String)str.get(2);
		String reverse=new StringBuffer(P).reverse().toString();
		for(int i=0;i<sigma;i++)
			index.put(mySigma[i], new Integer(index.size()));
		
		Prefix(reverse,0);
		ComputeNext(reverse,0);
		 int n1 =((String)str.get(0)).length() , 
	  		     n2=((String)str.get(1)).length(),
	  		     m1=((String)str.get(2)).length(), i, j, k1;
  	     D=new short [n1+2][n2+2][m1+2]; 
  	     
  	   for(k1=0;k1<=m1;k1++)
	  		  for(i=0;i<=n1;i++)
	  			  for(j=0;j<=n2;j++)
		            D[i][j][k1]=(short)(-1);
  	
  	   
		int len=L(((String)str.get(0)).length(), ((String)str.get(1)).length(), 0);
		 long endTime1 = new Date().getTime();
         System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));
	
		System.out.println("New DPlen "+len);
	}
	
	
	short L2(int i, int j, int k1, int k2){
		
		if(D1[i][j][k1][k2]!=-1)
			return D1[i][j][k1][k2];
		
    	if((k1==((String)str.get(2)).length())||(k2==((String)str.get(3)).length()))
			return D1[i][j][k1][k2]=(short)-99;
		else if(i==0 || j==0)
			return D1[i][j][k1][k2]=0;
		else if(((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1)){
			char c=((String)str.get(0)).charAt(i-1);
			int l1=next[0][k1][(Integer)index.get(c)], 
				l2=next[1][k2][(Integer)index.get(c)],
					temp;
			temp=Math.max(L2(i-1,j,k1, k2), L2(i,j-1,k1,k2));
			return D1[i][j][k1][k2]=(short)Math.max(L2(i-1,j-1,l1,l2)+1, temp);
		}
		else
			return D1[i][j][k1][k2]=(short)Math.max(L2(i-1,j,k1,k2), L2(i,j-1,k1,k2));
	}

void StrAlgo2(ArrayList input){
		
		long startTime1 = new Date().getTime();
		str=input;
		int n1 =((String)str.get(0)).length() , 
	  		     n2=((String)str.get(1)).length(),
	  		     m1=((String)str.get(2)).length(), 
	  		     m2=((String)str.get(3)).length(), 
	  		     i, j, k1, k2;
		
		for(i=0;i<sigma;i++)
			index.put(mySigma[i], new Integer(index.size()));
		
		for(i=2,j=0;i<4;i++){
			String P=(String)str.get(i);
			String reverse=new StringBuffer(P).reverse().toString();
			Prefix(reverse,j);
			ComputeNext(reverse,j++);
		}
  	     
		D1=new short [n1+1][n2+1][m1+1][m2+1]; 
  	     
  	   for(k1=0;k1<=m1;k1++)
  		 for(k2=0;k2<=m2;k2++)
	  		  for(i=0;i<=n1;i++)
	  			  for(j=0;j<=n2;j++)
		            D1[i][j][k1][k2]=(byte)(-1);
  	
  	   
		int len=L2(((String)str.get(0)).length(), ((String)str.get(1)).length(), 0,0);
		 long endTime1 = new Date().getTime();
         System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));
	
		System.out.println("New DPlen "+len);
	}


short L3(int i, int j, int k1, int k2, int k3){
	
	if(D2[i][j][k1][k2][k3]!=-1)
		return D2[i][j][k1][k2][k3];
	
	if((k1==((String)str.get(2)).length())||(k2==((String)str.get(3)).length())||(k3==((String)str.get(4)).length()))
		return D2[i][j][k1][k2][k3]=(short)-999;
	else if(i==0 || j==0)
		return D2[i][j][k1][k2][k3]=0;
	else if(((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1)){
		char c=((String)str.get(0)).charAt(i-1);
		int l1=next[0][k1][(Integer)index.get(c)], 
			l2=next[1][k2][(Integer)index.get(c)],
			l3=next[2][k3][(Integer)index.get(c)],
				temp;
		temp=Math.max(L3(i-1,j,k1, k2,k3), L3(i,j-1,k1,k2,k3));
		return D2[i][j][k1][k2][k3]=(short)Math.max(L3(i-1,j-1,l1,l2,l3)+1, temp);
	}
	else
		return D2[i][j][k1][k2][k3]=(short)Math.max(L3(i-1,j,k1,k2,k3), L3(i,j-1,k1,k2,k3));
}

void StrAlgo3(ArrayList input){
	
	long startTime1 = new Date().getTime();
	str=input;
	int n1 =((String)str.get(0)).length() , 
  		     n2=((String)str.get(1)).length(),
  		     m1=((String)str.get(2)).length(), 
  		     m2=((String)str.get(3)).length(), 
  		     m3=((String)str.get(4)).length(), 
  		     i, j, k1, k2, k3;
	
	for(i=0;i<sigma;i++)
		index.put(mySigma[i], new Integer(index.size()));
	
	for(i=2,j=0;i<5;i++){
		String P=(String)str.get(i);
		String reverse=new StringBuffer(P).reverse().toString();
		Prefix(reverse,j);
		ComputeNext(reverse,j++);
	}
	     
	D2=new short [n1+2][n2+2][m1+2][m2+2][m3+2]; 
	     
	   for(k1=0;k1<=m1;k1++)
		 for(k2=0;k2<=m2;k2++)
			 for(k3=0;k3<=m3;k3++)
  		  for(i=0;i<=n1;i++)
  			  for(j=0;j<=n2;j++)
	            D2[i][j][k1][k2][k3]=(short)(-1);
	
	   
	int len=L3(((String)str.get(0)).length(), ((String)str.get(1)).length(), 0,0,0);
	 long endTime1 = new Date().getTime();
     System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

	System.out.println("New DPlen "+len);
}
	

short L4(int i, int j, int k1, int k2, int k3, int k4){
	
	if(D3[i][j][k1][k2][k3][k4]!=-1)
		return D3[i][j][k1][k2][k3][k4];
	
	if((k1==((String)str.get(2)).length())||(k2==((String)str.get(3)).length())||(k3==((String)str.get(4)).length())||(k4==((String)str.get(5)).length()))
		return D3[i][j][k1][k2][k3][k4]=(byte)-999;
	else if(i==0 || j==0)
		return D3[i][j][k1][k2][k3][k4]=0;
	else if(((String)str.get(0)).charAt(i-1)==((String)str.get(1)).charAt(j-1)){
		char c=((String)str.get(0)).charAt(i-1);
		int l1=next[0][k1][(Integer)index.get(c)], 
			l2=next[1][k2][(Integer)index.get(c)],
			l3=next[2][k3][(Integer)index.get(c)],
			l4=next[3][k4][(Integer)index.get(c)],
				temp;
		temp=Math.max(L4(i-1,j,k1, k2,k3,k4), L4(i,j-1,k1,k2,k3,k4));
		return D3[i][j][k1][k2][k3][k4]=(byte)Math.max(L4(i-1,j-1,l1,l2,l3,l4)+1, temp);
	}
	else
		return D3[i][j][k1][k2][k3][k4]=(byte)Math.max(L4(i-1,j,k1,k2,k3,k4), L4(i,j-1,k1,k2,k3,k4));
}
void StrAlgo4(ArrayList input){
	
	long startTime1 = new Date().getTime();
	str=input;
	int n1 =((String)str.get(0)).length() , 
  		     n2=((String)str.get(1)).length(),
  		     m1=((String)str.get(2)).length(), 
  		     m2=((String)str.get(3)).length(), 
  		     m3=((String)str.get(4)).length(), 
  		     m4=((String)str.get(5)).length(), 
  		     i, j, k1, k2, k3, k4;
	
	for(i=0;i<sigma;i++)
		index.put(mySigma[i], new Integer(index.size()));
	
	for(i=2,j=0;i<6;i++){
		String P=(String)str.get(i);
		String reverse=new StringBuffer(P).reverse().toString();
		Prefix(reverse,j);
		ComputeNext(reverse,j++);
	}
	     
	D3=new byte [n1+1][n2+1][m1+1][m2+1][m3+1][m4+1]; 
	     
	   for(k1=0;k1<m1;k1++)
		 for(k2=0;k2<m2;k2++)
			 for(k3=0;k3<m3;k3++)
			   for(k4=0;k4<m4;k4++)
  		        for(i=0;i<=n1;i++)
  			     for(j=0;j<=n2;j++)
	              D3[i][j][k1][k2][k3][k4]=(short)(-1);
	
	   
	int len=L4(((String)str.get(0)).length(), ((String)str.get(1)).length(), 0,0,0,0);
	 long endTime1 = new Date().getTime();
     System.out.println("Total elapsed time in execution of DP CLCS  is :"+ (endTime1-startTime1));

	System.out.println("New DPlen "+len);
}
	
}

