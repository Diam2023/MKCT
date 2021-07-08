package top.monoliths.mkct.utils;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.File;
import java.io.RandomAccessFile;

/**
 * a test class for MikuClubCompressTools.
 */
public class MikuClubCompressToolsTest {

    /**
     * mikuclub head class.
     */
    static final String MIKUCLUB = "20151227";

    static final String PATH = "ano.zip";

    @Test
    void testChangeHeaderTo() {
        try {
            MikuClubCompressTools.changeHeaderTo(PATH);
            boolean condition = MikuClubCompressTools.getFileHeader(PATH).equals(MIKUCLUB);
            assertEquals(condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
    }

    @Test
    void testChangeHeaderBack() {
        try {
            MikuClubCompressTools.changeHeaderBack(PATH);
            boolean condition = MikuClubCompressTools.getFileHeader(PATH).equals(MIKUCLUB);
            assertEquals(!condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
    }

    @Test
    void testChangeTo() {
        try {
            // get randomAccessFile instance
            RandomAccessFile file = new RandomAccessFile(new File(PATH), "rws");
            // run changeBack
            MikuClubCompressTools.changeTo(file);
            // close file stream
            file.close();
            boolean condition = MikuClubCompressTools.getFileHeader(PATH).equals(MIKUCLUB);
            assertEquals(condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
    }

    @Test
    void testChangeBack() {
        try {
            // get randomAccessFile instance
            RandomAccessFile file = new RandomAccessFile(new File(PATH), "rws");
            // run changeBack
            MikuClubCompressTools.changeBack(file);
            // close file stream
            file.close();
            boolean condition = MikuClubCompressTools.getFileHeader(PATH).equals(MIKUCLUB);
            assertEquals(!condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
    }

    @Test
    void testBytesToHexString() {
        boolean condition = (MikuClubCompressTools.bytesToHexString(MikuClubCompressTools.getMikuclubHeader()).equals(MIKUCLUB));
        assertEquals(condition, true);
    }

    @Test
    void testGetFileHeader() {
        try {
            // run changeBack
            MikuClubCompressTools.changeHeaderTo(PATH);
            boolean condition = MikuClubCompressTools.getFileHeader(PATH).equals(MIKUCLUB);
            assertEquals(condition, true);
            MikuClubCompressTools.changeHeaderBack(PATH);
            condition = MikuClubCompressTools.getFileHeader(PATH).equals(MIKUCLUB);
            assertEquals(!condition, true);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(false, true);
        }
    }
}