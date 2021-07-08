package top.monoliths.mkct;

import org.junit.jupiter.api.Test;

import top.monoliths.mkct.utils.MikuClubCompressTools;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * Unit test for simple App.
 */
class AppTest {

    /**
     * mikuclub head class.
     */
    static final String MIKUCLUB = "20151227";

    /**
     * test main App.
     */
    @Test
    void testApp() {
        String[] args = new String[] {"to", "ano.zip"};
        try {
            App.main(args);
            boolean condition = MikuClubCompressTools.getFileHeader(args[1]).equals(MIKUCLUB);
            assertEquals(condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
        args[0] = "back";
        try {
            App.main(args);
            boolean condition = MikuClubCompressTools.getFileHeader(args[1]).equals(MIKUCLUB);
            assertEquals(!condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
    }
}
