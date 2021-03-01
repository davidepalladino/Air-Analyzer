<!doctype html>
<html>
<head>
    <title></title>
</head>
<body>
<table align="center" border="0" cellpadding="1" cellspacing="25" style="width: 75%;">
    <tbody>
    <tr>
        <td style="text-align: center; width: 300%; height: 100px; background-color: rgb(0, 109, 179);"><span style="color:#FFFFFF;"><span style="font-size:36px;"><span style="font-family:arial,helvetica,sans-serif;">AIR ANALYZER</span> </span> </span></td>
    </tr>
    <tr>
        <td style="text-align: center;">
            <p style="margin-left: 40px; text-align: left;"><span style="font-size:16px;"><span style="font-family:arial,helvetica,sans-serif;">Hi {{ $name }} {{ $surname }}</span>,<br />
			<span style="font-family:arial,helvetica,sans-serif;">Thank you to sign up on Air Analyzer. Our hope is that the application can help you to improve your home. To validate your new account please click the link below.</span></span></p>

            <p><strong><a href="http://airanalyzer.servehttp.com:50208/api/validateAirAnalyzer?hash={{ $hash }}" target="_blank"><span style="color:#006db3;"><span style="font-size:16px;"><span style="font-family:arial,helvetica,sans-serif;"><span style="background-color:#FFFFFF;">VALIDATE</span></span></span></span></a></strong></p>

            <p style="margin-left: 40px; text-align: left;"><span style="font-size:16px;"><span style="font-family:arial,helvetica,sans-serif;">Remember, your username is: {{ $username}}</span></span></p>
        </td>
    </tr>
    </tbody>
</table>
</body>
</html>
