package top.monoliths.mkct;

import top.monoliths.mkct.utils.MikuClubCompressTools;

/**
 * MKCT project for java runtime.
 * 
 * @author monoliths
 */
public final class App {

    /**
     * tag of changeHeaderTo.
     */
    private static final String TAGTO = "to";

    /**
     * rag of changeHeaderBack.
     */
    private static final String TAGBACK = "back";

    /**
     * action file path.
     */
    private static String path = "";

    /**
     * final class override public constructor.
     */
    private App() {
    }

    /**
     * main program for java runtime.
     * 
     * @param args The arguments of the program.
     */
    public static void main(String[] args) throws Exception {
        // if arguments are not null.
        if (args.length > 1) {
            // save path.
            path = args[1];
            // if path is not null.
            if (!"".equals(path)) {
                // out put origin header.
                System.out.println("origin header:" + MikuClubCompressTools.getFileHeader(path));
                if (TAGTO.equals(args[0])) {
                    // change header to.
                    MikuClubCompressTools.changeHeaderTo(path);
                } else if (TAGBACK.equals(args[0])) {
                    // chaneg header back.
                    MikuClubCompressTools.changeHeaderBack(path);
                }
                // out put new header
                System.out.println("new header:" + MikuClubCompressTools.getFileHeader(path));
            }
        }
    }
}
