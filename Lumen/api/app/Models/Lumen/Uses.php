<?php

namespace App\Models\Lumen;

use Illuminate\Database\Eloquent\Model;

class Uses extends Model {
    protected $table = "uses";
    public $timestamps = false;

    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = [
        'userId', 'serviceId'
    ];

    /**
     * The attributes excluded from the model's JSON form.
     *
     * @var array
     */
    protected $hidden = [

    ];
}
