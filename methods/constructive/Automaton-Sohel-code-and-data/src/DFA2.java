



import java.util.*;

       /** Deterministic Finite Automata */
        public class DFA2 implements  Cloneable  {

            int stateNo;        /** The number of states of the DFA */
            int startState;          /** The start state of the DFA */
            char [] sigma;           /** The alphabet of the DFA */
            HashMap map;
            int delta[][];         /** The transition function - stateNo x sigma.size matrix */
            HashSet accept;      /** The set of accept (final) states */

            /** Constructs an empty DFA */
             public DFA2() {
                stateNo = 0; //sigma = new char [26];
                map = new HashMap();
            }

           /** Constructs a DFA that is a copy of the specified DFA. */
            public DFA2(DFA2 d) {
                stateNo = d.stateNo;
                if (stateNo>0) {
                    startState = d.startState;
                    sigma = (char []) d.sigma.clone();
                    delta = (int[][]) d.delta.clone();
                    accept = (HashSet) d.accept.clone();}
      }

            /** Constructs a DFA with specified components. */
           public DFA2(int stateNo, int startState, char [] alpha,
                   int delta[][], HashSet finalStates){

              this .stateNo = stateNo;// stateNo
              this .startState = startState;// startState

              map = new HashMap();// sigma
              for(int i=0; i<alpha.length; i++)
                 this.addSymbol(alpha[i]);

              this .sigma =  alpha.clone();
              this .delta = (int[][]) delta.clone(); // delta
              this.accept =finalStates ; // accept


 }

  /* adds new symbol to sigma*/
  void addSymbol(char ch) {

  if(!map.containsKey(ch)){
      Character cObj = new Character(ch);// sigma.add(cObj);
      map.put(ch, new Integer(map.size()));
   }
}
/*index of letters in sigma i.e a=1, c=2, g=3, t=4 etc */
   public int idxSymbol(char ch)
   {
      return ((Integer)map.get(new Character(ch))).intValue();
   }

}


