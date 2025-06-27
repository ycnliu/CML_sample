import com.cml.wrapper.MotionSystemWrapper;

public class TestJNI {
    public static void main(String[] args) {
        MotionSystemWrapper wrapper = new MotionSystemWrapper();
        wrapper.initialize();
        System.out.println("JNI call completed.");
    }
}
