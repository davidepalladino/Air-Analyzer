<?php /** @noinspection ALL */

namespace App\Http\Controllers;

use App\Mail\UserValidation;
use App\Models\AirAnalyzer\Room;
use App\Models\Lumen\Service;
use App\Models\Lumen\User;
use App\Models\Lumen\Uses;
use Illuminate\Support\Facades\Auth;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;
use Illuminate\Support\Facades\Mail;
include ('/var/www/html/api/resources/views/showConfirmValidationPage.php');

class LumenController extends Controller {
    public function __construct() {
        $this->middleware('auth:api', ['except' => ['login', 'signupService', 'signupAirAnalyzer', 'validateAirAnalyzer', 'checkUsername', 'checkEmail']]);
    }

    /**
     * Get a JWT via given credentials.
     *
     * @param  Request  $request
     * @return Response
     */
    public function login(Request $request) {
        $this->validate($request, [
            'username' => 'required|string',
            'password' => 'required|string',
            'isActive' => 'required|numeric'
        ]);

        $credentials = $request->only(['username', 'password', 'isActive']);

        if (!$token = Auth::attempt($credentials, true)) {
            $user = User::where('username', $request->input('username'))->first();
            if ($user != null && $user->isActive == 0) {
                return response()->json(null, 204);
            } else if ($user == null) {
                return response()->json(null, 401);
            }

            return response()->json(null, 401);
        }

        return $this->respondWithToken($token);
    }

    /**
     * Store a new user.
     *
     * @param  Request  $request
     * @return Response
     */
    public function signupService(Request $request) {
        $this->validate($request, [
            'name' => 'required|string|unique:services',
        ]);

        try {
            $service = new Service;
            $service->name= $request->input('name');
            $service->save();

            return response()->json(null, 201);

        } catch (\Exception $e) {
            return response()->json( null, 500);
        }
    }

    /**
     * Store a new user.
     *
     * @param  Request  $request
     * @return Response
     */
    public function signupAirAnalyzer(Request $request) {
        $this->validate($request, [
            'username' => 'required|string|unique:users',
            'password' => 'required',
            'email' => 'required|email|unique:users',
            'name' => 'required|string',
            'surname' => 'required|string',
            'question1' => 'required|string',
            'question2' => 'required|string',
            'question3' => 'required|string',
            'answer1' => 'required|string',
            'answer2' => 'required|string',
            'answer3' => 'required|string',
        ]);

        $service = Service::where('name', 'airanalyzer')->where('isActive', 1)->first();
        if (($service != null) && ($service->isActive == true)) {
            try  {

                $email = $request->input('email');
                $user = new User;
                $user->username= $request->input('username');
                $user->password = $request->input('password');
                $user->email = $request->input('email');
                $user->name = $request->input('name');
                $user->surname = $request->input('surname');
                $user->validationCode = hash("sha256", $user->username);
                $user->question1 = $request->input('question1');
                $user->question2 = $request->input('question2');
                $user->question3 = $request->input('question3');
                $user->answer1 = app('hash')->make($request->input('answer1'));
                $user->answer2 = app('hash')->make($request->input('answer2'));
                $user->answer3 = app('hash')->make($request->input('answer3'));
                $user->save();

                Mail::to($email)->send(new UserValidation($user->name, $user->surname, $user->username, $user->validationCode));

                return response()->json(null, 201);
            } catch (\Exception $e) {
                return response()->json(null, 500);
            }
        } else {
            return response()->json(null, 403);
        }
    }

    public function validateAirAnalyzer(Request $request) {
        $this->validate($request, [
            'hash' => 'required|string',
        ]);

        $user = User::where('validationCode', $request->input('hash'))->first();
        $service = Service::where('name', 'airanalyzer')->first();
        $uses = Uses::where('userId', $user->id)->where('serviceId', $service->id)->first();

        if ($user != null && $uses == null) {
            /* Creating the database, the user and setting the right authorizations to him and 'airanalyzer'. */
            DB::select("CREATE DATABASE airanalyzer_" . $user->id);
            DB::select("CREATE USER '" . $user->username . "'@'%' IDENTIFIED WITH mysql_native_password BY '" . $user->password . "'");
            DB::select("GRANT SELECT, INDEX, SHOW VIEW, CREATE VIEW ON airanalyzer_" . $user->id . ".* TO '" . $user->username . "'@'%'");
            DB::select("GRANT ALL PRIVILEGES ON airanalyzer_" . $user->id . ".* TO 'airanalyzer'@'%'");

            /* Creating the tables and populating the 'Room' table. */
            DB::select("CREATE TABLE airanalyzer_" . $user->id . ".Room (
                ID INT PRIMARY KEY,
                Name VARCHAR(25),
                IsActive BOOLEAN NOT NULL
            )");
            DB::select("CREATE TABLE airanalyzer_" . $user->id . ".Measure (
                DateAndTime DATETIME NOT NULL,
                DayWeek INT NOT NULL,
                Room INT NOT NULL
                    REFERENCES Room(ID)
                    ON DELETE NO ACTION
                    ON UPDATE CASCADE,
                Temperature DECIMAL(5, 2) NOT NULL,
                Humidity DECIMAL(5,2) NOT NULL,
                PRIMARY KEY(DateAndTime, Room)
            )");
            for ($id = 1; $id < 10; $id++) {
                DB::table("airanalyzer_" . $user->id . ".Room")->insert([
                    'ID' => $id,
                    'IsActive' => 0
                ]);
            }

            /* Activating the user. */
            User::where('validationCode', $request->input('hash'))->update([
                'isActive' => 1,
                'password' => app('hash')->make($user->password)
            ]);


            $service = Service::where('name', 'airanalyzer')->where('isActive', 1)->first();

            $use = new Uses();
            $use->userId = $user->id;
            $use->serviceId = $service->id;
            $use->save();

            showConfirmValidationPage("Good work! Your validation is now complete. You can use your device and app to monitor it.");
        } else {
            showConfirmValidationPage("Hey! There is some error with the validation. Try again later or check the link used.");
        }
    }

    public function checkUsername(Request $request) {
        $this->validate($request, [
            'username' => 'required|string'
        ]);

        $user = User::where('username', $request->input('username'))->first();
        if ($user != null) {
            return response()->json(null, 201);
        } else {
            return response()->json(null, 204);
        }
    }

    public function checkEmail(Request $request) {
        $this->validate($request, [
            'email' => 'required|string'
        ]);

        $user = User::where('email', $request->input('email'))->first();
        if ($user != null) {
            return response()->json(null, 201);
        } else {
            return response()->json(null, 204);
        }
    }

    public function getUser(Request $request) {
        $user = Auth::user();

        if ($user->count() != 0) {
            return response()->json($user);
        } else {
            return response()->json(null, 204);
        }
    }
}
