package it.davidepalladino.airanalyzer.model;

import java.io.Serializable;

public class Login implements Serializable {
    private String username;
    private String password;
    private String isActive;

    public Login() {
        this.isActive = "1";
    }

    public Login(String username, String password) {
        this.username = username;
        this.password = password;
        this.isActive = "1";
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getIsActive() {
        return isActive;
    }

    public void setIsActive(String isActive) {
        this.isActive = isActive;
    }



    public class Response {
        private String token;

        public Response(String token) {
            this.token = token;
        }

        public String getToken() {
            return token;
        }

        public void setToken(String token) {
            this.token = token;
        }
    }
}