<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateUsesTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up() {
        Schema::create('uses', function (Blueprint $table) {
            $table->unsignedBigInteger('userId')->foreign('userId')->references('id')->on("users");
            $table->unsignedBigInteger('serviceId')->foreign('serviceId')->foreign()->references('id')->on("services");
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down() {
        Schema::dropIfExists('use_relation');
    }
}
