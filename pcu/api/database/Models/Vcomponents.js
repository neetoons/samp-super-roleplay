module.exports = (sequelize, DataTypes) => {
    return sequelize.define("vcomponents",
        {
            modelid: {
                type: DataTypes.INTEGER,
                allowNull: false,
                primaryKey: true
            },
            componentid: {
                type: DataTypes.INTEGER,
                allowNull: false,
                primaryKey: true
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};