<?php

namespace App\Mail;

use Illuminate\Bus\Queueable;
use Illuminate\Mail\Mailable;
use Illuminate\Queue\SerializesModels;

class UserValidation extends Mailable {
    use Queueable, SerializesModels;

    public $name;
    public $surname;
    public $username;
    public $hash;

    public function __construct($name, $surname, $username, $hash) {
        $this->name = $name;
        $this->surname = $surname;
        $this->username = $username;
        $this->hash = $hash;
    }

    public function build() {
        return $this->view('userValidation')->subject("Verify Your Account");
    }
}
