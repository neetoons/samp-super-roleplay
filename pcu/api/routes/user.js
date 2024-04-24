const { Router } = require("express");
const userRoute = Router();

// Modules
const { getAllInfoUser } = require("../Controllers/userController");

userRoute.get("/info", getAllInfoUser);

module.exports = { userRoute };
