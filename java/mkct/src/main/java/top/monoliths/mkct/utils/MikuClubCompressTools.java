package top.monoliths.mkct.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * miku club compress tools.
 *
 * @author monoliths
 */
public final class MikuClubCompressTools {

    /**
     * mikuclub.com date 2015 12 27.
     */
    private static final byte[] MIKUCLUB_HEADER = { 32, 21, 18, 39 };

    public static byte[] getMikuclubHeader() {
        return MIKUCLUB_HEADER;
    }

    /**
     * file origin head data.
     */
    private static byte[] originHead = new byte[4];

    /**
     * tool class.
     */
    private MikuClubCompressTools() {
        super();
    }

    /**
     * change file header to mikuclub header.
     *
     * @param filePath file path
     * @throws IOException ioexception
     */
    public static void changeHeaderTo(final String filePath) throws IOException {
        // get randomAccessFile instance
        RandomAccessFile file = new RandomAccessFile(new File(filePath), "rws");
        // run changeTo
        changeTo(file);
        // close file stream
        file.close();
    }

    /**
     * change file header to origin header.
     *
     * @param filePath file path
     * @throws IOException ioexception
     */
    public static void changeHeaderBack(final String filePath) throws IOException {
        // get randomAccessFile instance
        RandomAccessFile file = new RandomAccessFile(new File(filePath), "rws");
        // run changeBack
        changeBack(file);
        // close file stream
        file.close();
    }

    /**
     * change stream header to mikuclub header.
     *
     * @param target action file
     * @throws IOException ioexception
     */
    public final static void changeTo(final RandomAccessFile target) throws IOException {
        // get file size
        long length = target.length();
        // skip to file head
        target.seek(0);
        // get file head
        target.read(originHead, 0, 4);
        // resize file
        target.setLength(length + 4);
        // skip to file head
        target.seek(0);
        // overwrite head to mikuclub create date
        target.write(MIKUCLUB_HEADER);
        // skip to file last
        target.seek(length);
        // write origin head
        target.write(originHead);
    }

    /**
     * from mikuclub header to origin header.
     *
     * @param target action file.
     * @throws IOException ioexception
     */
    public final static void changeBack(final RandomAccessFile target) throws IOException {
        // get file size
        long length = target.length();
        // skip to last
        target.seek(length - 4);
        // get origin file head
        target.read(originHead, 0, 4);
        // skip to file head
        target.seek(0);
        // overwrite to origin head
        target.write(originHead);
        // seek to last
        target.seek(length - 4);
        // write origin head
        target.write(new byte[4]);
        // resize file / delete last 4 data
        target.setLength(length - 4);
    }

    /**
     * byte data to HEX string.
     *
     * @param src byte data
     * @return {@code String} HEX String
     */
    public final static String bytesToHexString(final byte[] src) {
        StringBuilder builder = new StringBuilder();
        if (src == null || src.length <= 0) {
            return null;
        }
        String hv;
        for (int i = 0; i < src.length; i++) {
            hv = Integer.toHexString(src[i] & 0xFF).toUpperCase();
            if (hv.length() < 2) {
                builder.append(0);
            }
            builder.append(hv);
        }
        return builder.toString();
    }

    /**
     * to get file header.
     *
     * @param filePath origin head of file path.
     * @return HEX data of header
     */
    public final static String getFileHeader(final String filePath) throws IOException {
        FileInputStream is = null;
        String value = null;
        try {
            is = new FileInputStream(filePath);
            byte[] b = new byte[4];
            is.read(b, 0, b.length);
            value = bytesToHexString(b);
        } catch (Exception e) {
            throw e;
        } finally {
            if (null != is) {
                try {
                    is.close();
                } catch (IOException e) {
                }
            }
        }
        return value;
    }
}

