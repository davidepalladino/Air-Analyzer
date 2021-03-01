<?php

/** @var \Laravel\Lumen\Routing\Router $router */

/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It is a breeze. Simply tell Lumen the URIs it should respond to
| and give it the Closure to call when that URI is requested.
|
*/

$router->get('/', function () use ($router) {
    return $router->app->version();
});

$router->group(['middleware' => 'auth', 'prefix' => 'api/airanalyzer'], function ($router) {
    $router->get('getActiveRooms', 'AirAnalyzerController@getActiveRooms');
    $router->get('getInactiveRooms', 'AirAnalyzerController@getInactiveRooms');
    $router->post('renameRoom', 'AirAnalyzerController@renameRoom');
    $router->post('addRoom', 'AirAnalyzerController@addRoom');
    $router->post('removeRoom', 'AirAnalyzerController@removeRoom');
    $router->get('getMeasureDateLatest', 'AirAnalyzerController@getMeasureDateLatest');
    $router->get('getMeasuresDateAverage', 'AirAnalyzerController@getMeasuresDateAverage');
});

$router->group(['prefix' => 'api'], function () use ($router) {
    $router->post('login', 'LumenController@login');
    $router->post('signupService', 'LumenController@signupService');
    $router->post('signupAirAnalyzer', 'LumenController@signupAirAnalyzer');
    $router->get('validateAirAnalyzer', 'LumenController@validateAirAnalyzer');
    $router->get('checkUsername','LumenController@checkUsername');
    $router->get('checkEmail','LumenController@checkEmail');
    $router->get('getUser', 'LumenController@getUser');
});
