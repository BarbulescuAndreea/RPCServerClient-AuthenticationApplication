struct Request_user_authentication {
    string userId<20>;
    string action<20>;
    string resource<20>;
};

struct Response_authentication {
    string authToken<30>;
};

struct Request_user_access {
    string userId<20>;
    string authToken<30>;
    string refreshToken<30>;
    int mode;
};

struct Response_user_access {
    string accessToken<30>;
    string refreshToken<30>;
    string message<50>;
    int opNumber;
};

struct Request_user_validate {
    string action<20>;
    string resource<20>;
    string accessToken<30>;
};

struct Response_user_validate {
    string info<50>;
};

program RESOURCE_MANAGER {
    version RESOURCE_MANAGER_VERSION {
        Response_authentication USER_AUTH(Request_user_authentication) = 1;
        Response_user_access USER_ACC(Request_user_access) = 2;
        Response_user_validate USER_VALIDATE(Request_user_validate) = 3;
    } = 1;
} = 0x2fffffff;

