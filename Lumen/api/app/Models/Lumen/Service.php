<?php

namespace App\Models\Lumen;

use Illuminate\Database\Eloquent\Model;

class Service extends Model {
    public $timestamps = false;
    protected $attributes = [
        'isActive' => 1
    ];

    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = [
        'id', 'name', 'isActive'
    ];

    /**
     * The attributes excluded from the model's JSON form.
     *
     * @var array
     */
    protected $hidden = [

    ];
}
