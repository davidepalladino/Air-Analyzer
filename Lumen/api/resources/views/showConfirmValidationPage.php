<?php
    function showConfirmValidationPage($message) {
        ?>
        <!doctype html>
        <html>
        <head>
            <title>HTML Editor - Full Version</title>
        </head>
        <body>
        <table align="center" border="0" cellpadding="1" cellspacing="25" style="width:50%;">
            <tbody>
            <tr>
                <td style="text-align: center; width: 300%; height: 100px; background-color: rgb(0, 109, 179);"><span style="color:#FFFFFF;"><span style="font-size:36px;"><span style="font-family:arial,helvetica,sans-serif;">AIR ANALYZER</span></span></span></td>
            </tr>
            <tr>
                <td style="text-align: center;">
                    <p><span style="font-size:20px;"><span style="font-family:arial,helvetica,sans-serif;"><?php echo $message ?></span></span></p>
                </td>
            </tbody>
        </table>
        </body>
        </html>
        <?php
    }
?>
