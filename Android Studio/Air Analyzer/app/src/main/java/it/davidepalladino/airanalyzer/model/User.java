package it.davidepalladino.airanalyzer.model;

public class User {
    private String id;
    private String username;
    private String email;
    private String name;
    private String surname;
    private String question1;
    private String question2;
    private String question3;

    public User(String id, String username, String email, String name, String surname, String question1, String question2, String question3) {
        this.id = id;
        this.username = username;
        this.email = email;
        this.name = name;
        this.surname = surname;
        this.question1 = question1;
        this.question2 = question2;
        this.question3 = question3;
    }
}