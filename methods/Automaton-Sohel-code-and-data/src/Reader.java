
import java.io.*;
import java.util.*;

public class Reader {
	@SuppressWarnings({ "FieldCanBeLocal", "UnusedDeclaration" })
	InputReader in;
	PrintWriter output, out;
	ArrayList inputStr = new ArrayList();
	ArrayList pattern = new ArrayList();
	ArrayList totalStr = new ArrayList();
	ArrayList restrictStr = new ArrayList();
	String patternBuf;
	char mySigma[] = { 'a', 'c', 'g', 't' };
	HashSet<Character> readSigma=new HashSet<Character>();
	int patternLen;

	/*
	 * public static void main(String[] args) { Reader r=new Reader();
	 * r.readInput(); //r.run();
	 * 
	 * r.randomStr(10); r.randomStr(5); r.exit();
	 * 
	 * }
	 */
	void readInput() {
		@SuppressWarnings({ "UnusedDeclaration" })
		String id = getClass().getName().toLowerCase();

		try {
			System.setIn(new FileInputStream("d23_7.txt"));

		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		in = new InputReader(System.in);
	}

	void readInput(String FileName) {
		@SuppressWarnings({ "UnusedDeclaration" })
		String id = getClass().getName().toLowerCase();

		try {
			System.setIn(new FileInputStream(FileName));

		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		in = new InputReader(System.in);
	}

	void giveInput() {
		@SuppressWarnings({ "UnusedDeclaration" })
		String id = getClass().getName().toLowerCase();

		try {

			// System.setOut(new PrintStream(new FileOutputStream("test4200.txt")));
			output = new PrintWriter(new FileWriter("ds02.txt"));
		} catch (Exception e) {
			throw new RuntimeException(e);
		}

		// output = new PrintWriter(System.out);

	}

	@SuppressWarnings("empty-statement")
	void run() {
		//first row example
		//|S|=2	Sigma=20	|P|=6	k=4
		var strCountStr = in.readString().replace("|S|=", "");
		int strCount = Integer.parseInt(strCountStr);
		in.readString(); //skip sigma
		in.readString(); //skip |P|
		var restrictStrCountStr = in.readString().replace("k=","");
		int restrictStrCount = Integer.parseInt(restrictStrCountStr);
		;
		for (int i = 0; i < strCount; i++) {
			in.readString(); //skip len
			var str = in.readString();
			for(int k=0; k<str.length(); k++)
				readSigma.add(str.charAt(k));
			inputStr.add(str);
			totalStr.add(inputStr.get(i));
		}
		for (int i = 0; i < restrictStrCount; i++) {
			in.readString(); //skip len
			var str = in.readString();
			for(int k=0; k<str.length(); k++)
				readSigma.add(str.charAt(k));
			pattern.add(str);
			totalStr.add(pattern.get(i));
		}
	}

	String concatInput() {
		String str = (String) inputStr.get(0);
		for (int i = 1; i < inputStr.size(); i++) {
			str = str + '#';
			str += inputStr.get(i);
		}
		return str;
	}

	void randomStr(int length) {

		char buf[] = new char[length + 1];
		Arrays.fill(buf, '-');
		int random;

		for (int i = 0; i < length; i++) {
			random = (int) (Math.random() * 4);// random is between 1 & 4
			buf[i] = mySigma['a' + random];

		}
		buf[length] = '\0';
		output.println(buf);
		// String s="abc";
//Min + (int)(Math.random() * ((Max - Min) + 1))

	}

	void generatePattern(int len, int count) {

		int i, j, random;
		patternLen = len;

		restrictStr.clear();

		for (i = 0; i < count; i++) {
			char buf[] = new char[len + 1];
			for (j = 0; j < len; j++) {
				random = (int) (Math.random() * 20);
				buf[j] = mySigma['A' + random];
			}

			if (count == 0)
				patternBuf = new String(buf);
			else
				patternBuf += new String(buf);
			buf[len] = '\0';
			restrictStr.add(buf);

		}

	}

	void randStr(int len) {

		String buf = null;
		int i, strLen = len - patternLen * restrictStr.size(), randIndx, stIndex;
		buf = String.copyValueOf((char[]) restrictStr.get(0), 0, patternLen);
		// buf.

		for (i = 1; i < restrictStr.size(); i++) {
			buf = buf.concat(String.copyValueOf((char[]) restrictStr.get(i), 0, patternLen));
		}

		// System.out.println(buf);
		// System.out.println(strLen);
		// output.println(buf);

		// System.out.println(buf);
		// buf;//=patternBuf;
		for (i = 0; i < strLen; i++) {
			randIndx = (int) (Math.random() * (buf.length() - 1));
			stIndex = (randIndx == 0) ? 1 : randIndx;
			randIndx = (randIndx - 1) < 0 ? 0 : randIndx - 1;

			String s1 = buf.substring(0, randIndx + 1);
			String s2 = buf.substring(stIndex);
			// System.out.println("1st buf "+buf.length()+" s1 "+s1+" s2 "+s2);
			buf = s1 + mySigma[(int) (Math.random() * 4)] + s2;
			// System.out.println("rIn "+randIndx+"st "+stIndex+" "+buf.length());

		}

		output.println(buf);

	}

	void generateStr(int len, int count) {

		int i;

		output.println(count);
		output.println(restrictStr.size());

		for (i = 0; i < count; i++)
			randStr(len);

		for (i = 0; i < restrictStr.size(); i++)
			output.println((char[]) (restrictStr.get(i)));

	}

	@SuppressWarnings({ "UnusedDeclaration" })
	void exit() {
		output.close();
		System.exit(0);
	}

	@SuppressWarnings({ "UnusedDeclaration" })
	static class InputReader {
		InputStream stream;
		byte[] buf = new byte[1024];
		int curChar, numChars;

		InputReader(InputStream stream) {
			this.stream = stream;
		}

		int read() {
			if (numChars == -1)
				throw new InputMismatchException();
			if (curChar >= numChars) {
				curChar = 0;
				try {
					numChars = stream.read(buf);
				} catch (IOException e) {
					throw new InputMismatchException();
				}
				if (numChars <= 0)
					return -1;
			}
			return buf[curChar++];
		}

		String readString() {
			int c = read();
			while (isSpaceChar(c))
				c = read();
			StringBuffer res = new StringBuffer();
			do {
				res.appendCodePoint(c);
				c = read();
			} while (!isSpaceChar(c));
			return res.toString();
		}

		boolean isSpaceChar(int c) {
			return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1 || c == '\0';
		}

		int readInt() {
			int c = read();
			while (isSpaceChar(c))
				c = read();
			int sgn = 1;
			if (c == '-') {
				sgn = -1;
				c = read();
			}
			int res = 0;
			do {
				if (c < '0' || c > '9')
					throw new InputMismatchException();
				res *= 10;
				res += c - '0';
				c = read();
			} while (!isSpaceChar(c));
			return res * sgn;
		}

	}

}
