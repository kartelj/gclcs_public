


public class Queue2<T> {
   private int head = 0;
   private int tail = 0;
   public long size = 0L;
   private Key queue[];//=new Key[qSize];

   Queue2(){
    queue=new Key[Main.maxQueueSize];
    for(int i=0;i<Main.maxQueueSize;i++)
        queue[i]=new Key();

   }


    public void enqueue(int f, int l) {
      if (tail+1==head ||(tail+1==Main.maxQueueSize && head==0)) {
         System.out.println("Q full");
          return;
      }
      if(++tail==Main.maxQueueSize)tail=0;//tail++;
      queue[tail].first=f;
      queue[tail].last=l;
      size++;
   }

   public Key dequeue() throws IllegalArgumentException {
      if (head == tail) {
         throw new IllegalArgumentException();
      }
      if(++head==Main.maxQueueSize)head=0;//head++;
      size--;
      return queue[head];
   }

   public long size() {
      return size;
   }

   public void init(){
     head=tail=0;
     size=0;
   }
}

class myQueue<T> {
   private int head = 0;
   private int tail = 0;
   public long size = 0L;
   public static final int qSize=50000;
   private int queue[];//=new Key[qSize];

   myQueue(){
    queue=new int[qSize];

   }


    public void enqueue(int key) {
      if (tail+1==head ||(tail+1==qSize && head==0)) {
         System.out.println("Q full");
          return;
      }
      if(++tail==qSize)tail=0;//tail++;
      queue[tail]=key;
      size++;
   }

   public int dequeue() throws IllegalArgumentException {
      if (head == tail) {
         throw new IllegalArgumentException();
      }
      if(++head==qSize)head=0;//head++;
      size--;
      return queue[head];
   }

   public long size() {
      return size;
   }

   public void init(){
     head=tail=0;
     size=0;
   }
}


class Key
{
  public  int first;
  public  int last;


  Key(final int firstnum, final int lastnum)
  {
    first = firstnum;
    last = lastnum;
  }

  Key(){
   first=-1;
   last=-1;
  }

  void set(int f, int l){
   first=f;
   last=l;
 }

  @Override
  public boolean equals(final Object o)
  {

    if (!(o instanceof Key))
    {
      return false;
    }

     Key key = (Key)o;

    // (If some field is a primitive, start with that one, it's faster.)
     return first==key.first && last==key.last;
  }

 @Override
  public int hashCode()//
  { // This could potentially be calculated once and stored in a private field.
       return Integer.toString(first).hashCode() +", ".hashCode() +Integer.toString(last).hashCode();
  }
}


