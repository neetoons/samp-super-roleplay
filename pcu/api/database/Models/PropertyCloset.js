module.exports = (sequelize, DataTypes) => {
    return sequelize.define("property_closet",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            type: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            int: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            int_extra: {
                type: DataTypes.INTEGER,
                allowNull: false
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};